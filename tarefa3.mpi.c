#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include<tour.h>
#include<tour-mpi.h>
#include<tour-queue.h>
#include<tour-stack.h>

void _print_tour(tour_t tour)
{
  printf("{len: %d, cost: %d, tour: [%d", tour->len, tour->cost, tour->tour[0]);
  for (int i = 1; i < tour->len; i++)
  {
    printf(", %d", tour->tour[i]);
  }
  printf("]}\n");
}

#define CPU_NUM 4

int digraph[MAX_CITY_NUM][MAX_CITY_NUM];
int n;
int homecity = 0;
tour_t best_tour;

int process_count = 0;
pthread_t* thread_handles;
pthread_mutex_t best_tour_mutex; /*mutex */


const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}


void init();
void bfs(tour_queue_t queue, int size); // Breadth first search, used to expand enough nodes to allocate threads
void* tsp(void* stack, int process_count, int rank);

void init(FILE* stream)
{
    char line[1024];
    fgets(line, 1024, stream);
    char* tmp = strdup(line);

    n = atoi(getfield(tmp, 1));
    printf("Number of cities %d\n", n);


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            digraph[i][j] = i == j ? 0 : INT_MAX;
        }
    }

    int _row = 0;
    char* _tmp;
    while (fgets(line, 1024, stream))
    {
        for(int _col = 0; _col < n; _col++)
        {
            _tmp = strdup(line);
            digraph[_row][_col] = atoi(getfield(_tmp, _col + 1));
            // printf("Field %d would be %d\n ", _col + 1, digraph[_row][_col] );
        }
        // NOTE strtok clobbers tmp
        _row++;
    }
    free(_tmp);

    best_tour = malloc(sizeof(struct tour_t));
    best_tour->cost = INT_MAX;
}
// Breadth first search
void bfs(tour_queue_t queue, int size){
    tour_t cur_tour = create_tour();

    append_city(cur_tour, homecity, digraph[homecity][homecity]);
    enqueue_copy(queue, cur_tour);
    free(cur_tour);

    bool ready = false;

    while (!queue_empty(queue) && !ready) {
        cur_tour = dequeue(queue);

        if (cur_tour->len == n) {
            // Determine whether it can be updated
            int city = get_last_city(cur_tour);

            if (digraph[city][homecity] != INT_MAX
                && cur_tour->cost + digraph[city][homecity] < best_tour->cost) {
                // Optimal cost of renewal
                best_tour->cost = cur_tour->cost + digraph[city][homecity];
                // Update the optimal path
                copy_tour(best_tour, cur_tour);
                append_city(best_tour, homecity, digraph[get_last_city(best_tour)][homecity]);
            }

            continue;
        }

        for (int nbr = n - 1; nbr >= 1; --nbr) {
            // Skip cur_ Existing nodes in the tour
            if (find_in_tour(cur_tour, nbr) != -1) {
                continue;
            }

            int new_cost = cur_tour->cost + digraph[get_last_city(cur_tour)][nbr];

            // Branch boundary, skip the node that can not expand to a better solution
            if (new_cost >= best_tour->cost) {
                continue;
            }

            append_city(cur_tour, nbr, digraph[get_last_city(cur_tour)][nbr]);
            enqueue_copy(queue, cur_tour);
            remove_last_city(cur_tour, digraph[cur_tour->tour[cur_tour->len - 2]][cur_tour->tour[cur_tour->len - 1]]);

            if (queue_size(queue) >= size) {
                ready = true;
                break;
            }
        }

        free(cur_tour);
    }
}

void* tsp(void* stack, int process_count, int rank)
{
    tour_stack_t my_stack = (tour_stack_t)stack;
    tour_t cur_tour;

    printf("[%s %d %ld] my_stack.size = %d\n", __FILE__, __LINE__, pthread_self(), stack_size(stack));

    while (!stack_empty(my_stack)) {
        cur_tour = pop(my_stack);

        if (cur_tour->len == n) {
            // Determine whether it can be updated
            int city = get_last_city(cur_tour);

            if (digraph[city][homecity] != INT_MAX
                && cur_tour->cost + digraph[city][homecity] < best_tour->cost) {

                // In the process of waiting for the lock, other processes 
                // may have modified the optimal path, so it needs to be determined again
                for (int i = 0; i < process_count; i++)
                {
                    int flag;
                    tour_t _t = receive_tour_package(i + 1, &flag);
                    best_tour = (flag && _t->cost < best_tour->cost) ? _t : best_tour;
                }
                if (cur_tour->cost + digraph[city][homecity] < best_tour->cost) {
                    // Optimal cost of renewal
                    best_tour->cost = cur_tour->cost + digraph[city][homecity];
                    // Update the optimal path
                    copy_tour(best_tour, cur_tour);
                    append_city(best_tour, homecity, digraph[get_last_city(best_tour)][homecity]);
                    
                    for (int i = 0; i < process_count; i++)
                    {
                        if(i + 1 != rank)
                            send_tour_package(create_tour_package(best_tour, i + 1));
                    }
                }
            }
            continue;
        }

        for (int nbr = n - 1; nbr >= 1; --nbr) {
            // Skip cur_ Existing nodes in the tour
            if (find_in_tour(cur_tour, nbr) != -1) {
                continue;
            }
            int new_cost = cur_tour->cost + digraph[get_last_city(cur_tour)][nbr];
            // Branch boundary, skip the node that can not expand to a better solution
            if (new_cost >= best_tour->cost) {
                continue;
            }
            append_city(cur_tour, nbr, digraph[get_last_city(cur_tour)][nbr]);
            push_copy(my_stack, cur_tour);
            remove_last_city(cur_tour, digraph[cur_tour->tour[cur_tour->len - 2]][cur_tour->tour[cur_tour->len - 1]]);
        }
        free(cur_tour);
    }
    return NULL;
}

int main(int argc, char** argv) {
  { 
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    printf("PID %d on %s ready for attach\n", getpid(), hostname);
    fflush(stdout);
  //   int i=0;
  //   while (0 == i)
  //     sleep(2);
  }
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  FILE* stream = fopen("./datasets/P01/p01_d.txt", "r");
  init(stream);


  if (world_rank == 0) {
    sleep(2);
    tour_queue_t queue = create_queue();
    bfs(queue, world_size - 1);
    process_count = queue_size(queue);
    printf("Process count = %d\n", process_count);

    for (int i = 0; i < process_count; ++i) {
        tour_stack_t my_stack = alloc_stack();
        tour_t t = dequeue(queue);
        push(my_stack, t);
        // Send Stack here
        tour_stack_package_t pack = create_tour_stack_package(my_stack, i + 1);
        send_tour_stack_package(pack);
    }
  } else {
    // int i=0;
    // while (0 == i)
    //     sleep(2);
    tour_stack_t my_stack = probe_receive_tour_stack_package(0);
    tsp(my_stack, world_size - 1, world_rank);
    _print_tour(best_tour);
    output_tour(best_tour);
  }
  MPI_Finalize();
}