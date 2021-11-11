#include <tour-mpi.h>

tour_package_t create_tour_package(tour_t tour, int dest)
{
  tour_package_t pack = malloc(sizeof(struct tour_package_t));
  pack->tag = DEFAULT_TOUR_PACKAGED_TAG;
  pack->dest = dest;

  int position = 0;
  int pack_size = sizeof(int) * (MAX_CITY_NUM + 2);
  MPI_Pack(tour->tour, MAX_CITY_NUM, MPI_INT, pack->buffer, pack_size, &position, MPI_COMM_WORLD);
  MPI_Pack(&tour->len, 1, MPI_INT, pack->buffer, pack_size, &position, MPI_COMM_WORLD);
  MPI_Pack(&tour->cost, 1, MPI_INT, pack->buffer, pack_size, &position, MPI_COMM_WORLD);
  pack->buffer_size = position;

  return pack;
}

int send_tour_package(tour_package_t package)
{
  return MPI_Send(package->buffer, package->buffer_size, MPI_PACKED, package->dest, package->tag, MPI_COMM_WORLD);
}

tour_t receive_tour_package(int source_rank)
{
    int number_amount;
    int flag;
    MPI_Status status;
    // Probe for an incoming message from process zero
    MPI_Iprobe(source_rank, DEFAULT_TOUR_PACKAGED_TAG, MPI_COMM_WORLD, &flag, &status);
    if(flag)
    {
      // When probe returns, the status object has the size and other
      // attributes of the incoming message. Get the message size
      MPI_Get_count(&status, MPI_PACKED, &number_amount);
      // Allocate a buffer to hold the incoming numbers
      int* buf = (int*) malloc(number_amount);
      MPI_Recv(buf, number_amount, MPI_PACKED, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      int position = 0;
      tour_t _tour = create_tour();
      MPI_Unpack(buf, number_amount, &position, _tour->tour, MAX_CITY_NUM, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buf, number_amount, &position, &_tour->len, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buf, number_amount, &position, &_tour->cost, 1, MPI_INT, MPI_COMM_WORLD);
      return _tour;
    }
    return NULL;
}