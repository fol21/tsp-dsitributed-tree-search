#ifndef TOUR_MPI_H
#define TOUR_MPI_H

#include<tour.h>
#include<tour-stack.h>
#include<mpi.h>

#define DEFAULT_TOUR_PACKAGED_TAG MAX_CITY_NUM + 2
#define DEFAULT_TOUR_STACK_PACKAGED_TAG (MAX_STACK_SIZE * (MAX_CITY_NUM + 2)) + 1

struct tour_package_t {
    int buffer[MAX_CITY_NUM + 2];
    int buffer_size;
    int tag;
    int dest;
};
typedef struct tour_package_t* tour_package_t;

struct tour_stack_package_t {
    int buffer[(MAX_STACK_SIZE * (MAX_CITY_NUM + 2)) + 1];
    int buffer_size;
    int tag;
    int dest;
};
typedef struct tour_stack_package_t* tour_stack_package_t;


tour_package_t create_tour_package(tour_t tour, int dest);
tour_stack_package_t create_tour_stack_package(tour_stack_t stack, int dest);
int send_tour_package(tour_package_t package);
int send_tour_stack_package(tour_stack_package_t package);
tour_t receive_tour_package(int source_rank);
tour_stack_t receive_tour_stack_package(int source_rank);

#endif // !TOUR_MPI_H
