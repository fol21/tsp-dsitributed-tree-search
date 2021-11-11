#ifndef TOUR_MPI_H
#define TOUR_MPI_H

#include<tour.h>
#include<mpi.h>

#define DEFAULT_TOUR_PACKAGED_TAG MAX_CITY_NUM + 2

struct tour_package_t {
    int buffer[MAX_CITY_NUM + 2];
    int buffer_size;
    int tag;
    int dest;
};
typedef struct tour_package_t* tour_package_t;

tour_package_t create_tour_package(tour_t tour, int dest);
int send_tour_package(tour_package_t package);
tour_t receive_tour_package(int source_rank);

#endif // !TOUR_MPI_H
