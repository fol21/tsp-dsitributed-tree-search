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

void* send(void* args)
{
    int* rank = (int*) args;
    int number = *rank;
    printf("thread assigned to sending to rank: %d\n", *rank);
    sleep((*rank));
    MPI_Send(
      /* data         = */ rank, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ *rank, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
    
    return NULL;
}
void* receive(void* args)
{
    int* rank = (int*) args;
    int number = 0;
    int flag = 0;
    MPI_Status status;
    printf("Process %d ready to receive.\n", *rank);

    while(1)
    {
      // while(!flag)
      // {
      //   MPI_Iprobe( 0, 0, MPI_COMM_WORLD, &flag, &status);
      // }
      MPI_Recv(
        /* data         = */ &number, 
        /* count        = */ 1, 
        /* datatype     = */ MPI_INT, 
        /* source       = */ 0, 
        /* tag          = */ 0, 
        /* communicator = */ MPI_COMM_WORLD, 
        /* status       = */ MPI_STATUS_IGNORE
      );
      printf("Process %d received number {%d} from process 0\n", *rank, number);
    }
    return NULL;
}

void send_tour(tour_t tour, int dest) {
  int position = 0;
  int* config_buf[MAX_CITY_NUM + 2];
  MPI_Pack(tour->tour, MAX_CITY_NUM, MPI_INT, config_buf, MAX_CITY_NUM + 2, &position, MPI_COMM_WORLD);
  MPI_Pack(&tour->len, 1, MPI_INT, config_buf, MAX_CITY_NUM + 2, &position, MPI_COMM_WORLD);
  MPI_Pack(&tour->cost, 1, MPI_INT, config_buf, MAX_CITY_NUM + 2, &position, MPI_COMM_WORLD);
  MPI_Send(config_buf, position, MPI_PACKED, dest, 0, MPI_COMM_WORLD);
}

void receive_tour()
{
    int number_amount;
    int flag;
    MPI_Status status;
    // Probe for an incoming message from process zero
    MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, &status);

    // When probe returns, the status object has the size and other
    // attributes of the incoming message. Get the message size
    MPI_Get_count(&status, MPI_INT, &number_amount);

    // Allocate a buffer to hold the incoming numbers
    int* number_buf = (int*)malloc(sizeof(int) * number_amount);
}

int main(int argc, char** argv) {
  { 
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    printf("PID %d on %s ready for attach\n", getpid(), hostname);
    fflush(stdout);
    int i=0;
    while (0 == i)
      sleep(5);
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
    sleep(10);
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
    pthread_join(&receive_thread, NULL);
  }
  MPI_Finalize();
}