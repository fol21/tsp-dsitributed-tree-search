#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>

#include<tour.h>
#include<tour-queue.h>
#include<tour-stack.h>
#include<utils.h>

#include <benchmark.h>

int CPU_NUM = 1;

double digraph[MAX_CITY_NUM][MAX_CITY_NUM];
int n;
int homecity = 0;
tour_t best_tour;


int thread_count = 0;
pthread_t* thread_handles;
pthread_mutex_t best_tour_mutex; /*mutex */

void _print_tour(tour_t tour)
{
  printf("{len: %d, cost: %.2f, tour: [%d", tour->len, tour->cost, tour->tour[0]);
  for (int i = 1; i < tour->len; i++)
  {
    printf(", %d", tour->tour[i]);
  }
  printf("]}\n");
}

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
void* tsp(void* stack);

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
void bfs(tour_queue_t queue, int size)
{
    tour_t cur_tour = create_tour();

    append_city(cur_tour, homecity, digraph[get_last_city(cur_tour)][homecity]);
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

            double new_cost = cur_tour->cost + digraph[get_last_city(cur_tour)][nbr];

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

void* tsp(void* stack)
{
    tour_stack_t my_stack = (tour_stack_t)stack;
    tour_t cur_tour;

    printf("[%s %d %ld] my_stack.size = %d\n", __FILE__, __LINE__, pthread_self(), stack_size(stack));

    while (!stack_empty(my_stack)) {
        cur_tour = pop(my_stack);

        if (cur_tour->len == n) {
            // Determine whether it can be updated
            int city = get_last_city(cur_tour);

            // printf("[%ld] stack size: %d\n", pthread_self(), stack_size(my_stack));
            if (digraph[city][homecity] != INT_MAX
                && cur_tour->cost + digraph[city][homecity] < best_tour->cost) {

                // Lock
                pthread_mutex_lock(&best_tour_mutex);

                // In the process of waiting for the lock, other threads may have modified the optimal path, so it needs to be determined again
                if (cur_tour->cost + digraph[city][homecity] < best_tour->cost) {
                    // Optimal cost of renewal
                    best_tour->cost = cur_tour->cost + digraph[city][homecity];
                    printf("[%ld] Best tour found: cost = %.2f, stack size: %d\n", pthread_self(), best_tour->cost, stack_size(my_stack));
                    // Update the optimal path
                    copy_tour(best_tour, cur_tour);
                    append_city(best_tour, homecity, digraph[get_last_city(best_tour)][homecity]);
                }

                // Release the lock
                pthread_mutex_unlock(&best_tour_mutex);
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

    return best_tour;
}


void* routine(void* stack)
{
    tsp(stack);
    // _print_tour(best_tour);
    // output_tour(best_tour);
    return NULL;
}

void main_routine(void* args)
{    
    tour_queue_t queue = create_queue();
    bfs(queue, CPU_NUM);

    thread_count = queue_size(queue);
    printf("Thread count = %d\n", thread_count);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    

    for (int i = 0; i < thread_count; ++i) {
        tour_stack_t my_stack = alloc_stack();
        tour_t t = dequeue(queue);
        push(my_stack, t);
        pthread_create(&thread_handles[i], NULL, routine, my_stack);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(thread_handles[i], NULL);
    }

    printf("====== Best tour ======\n");
    _print_tour(best_tour);
    output_tour(best_tour);

    free(queue);
}


int main(int argc, char const *argv[])
{
    CPU_NUM = atoi(argv[1]);

    bool is_coord = atoi(argv[2]);
    FILE* stream = fopen(argv[3], "r");
    if(is_coord)
    {
        Digraph_s* _d = coord_to_digraph(stream, euclidian_distance);
        n = _d->num_cities;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                digraph[i][j] = (_d->digraph)[i][j];
    
        best_tour = malloc(sizeof(struct tour_t));
        best_tour->cost = INT_MAX;
    }
    else
        init(stream);

    double time = stopwatch(main_routine, NULL);
    printf("====== Performance =======\n");
    printf("Total calculation time: %.2f seconds\n", time);
    return 0;
}