// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// to process 1.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include<tour.h>
#include<tour-mpi.h>

void _print_tour(tour_t tour)
{
  printf("{len: %d, cost: %d, tour: [%d", tour->len, tour->cost, tour->tour[0]);
  for (int i = 1; i < tour->len; i++)
  {
    printf(", %d", tour->tour[i]);
  }
  printf("]}\n");
}

void* send(void* args)
{
    int* rank = (int*) args;
    printf("thread assigned for sending to rank: %d\n", *rank);
    sleep((*rank));
    tour_t cur_tour = create_tour();

    append_city(cur_tour, 1, 4);
    append_city(cur_tour, 2, 5);
    append_city(cur_tour, 3, 10);
    append_city(cur_tour, 4, 2);
    append_city(cur_tour, 1, 3);
    send_tour_package(create_tour_package(cur_tour, *rank));
    return NULL;
}
void* receive(void* args)
{
    int* rank = (int*) args;
    printf("Process %d ready to receive.\n", *rank);

    while(1)
    {
      tour_t tour =  receive_tour_package(0);
      if(tour)
      {
          printf("Process %d received the following tour from process 0: ", *rank);
          _print_tour(tour);
      }

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

  if (world_rank == 0) {
    sleep(3);
    pthread_t* thread_handles = malloc((world_size - 1) * sizeof(pthread_t));
    int i;
    for (i = 1; i < world_size; i++) {
        pthread_create(&thread_handles[i - 1], NULL, send, &i);
    }

    for (i = 1; i < world_size; i++) {
        pthread_join(thread_handles[i - 1], NULL);
    }
  } else {
    pthread_t* receive_thread = malloc(sizeof(pthread_t));
    pthread_create(receive_thread, NULL, receive, &world_rank);
    pthread_join((pthread_t) &receive_thread, NULL);
  }
  MPI_Finalize();
}