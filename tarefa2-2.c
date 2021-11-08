// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/syscall.h>

// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// #include <queue.h>
// #include <utils.h>
// #include <benchmark.h>
// #include <adaptative-quadrature.h>

// #ifdef _OPENMP
// #include <omp.h>
// #endif

// typedef struct _r_args {adaptavive_quadrature_args aq_args; int num_intervals;} _r_args;

// void omp_routine(void* arg)
// {
//     _r_args* args = (_r_args*) arg;
//     double total = 0;
//     Queue_v* queue = createQueue(1024 * 1024  / sizeof(int));

//     omp_adaptavive_quadrature_admin(&total, &(args->aq_args), queue, 1);
//     printf("total: %2f\n", total);
// }

// int main(int argc, char *argv[])
// {
//     //Argument Input
//     double L = (double) strtof(argv[1], NULL);
//     double R = (double) strtof(argv[2], NULL);
//     double A = (double) strtof(argv[3], NULL);
//     int T = (int) strtol(argv[4], NULL , 10);
//     printf("Parametros l, r, aproximation, number of threads = %.1f %.1f %.9f %d\n", L, R, A, T);
//     omp_set_num_threads(T);
//     // omp_set_num_threads(4);
//     omp_set_nested(1);

//     adaptavive_quadrature_args aq_args = {L, R, abs_sinc, A};
//     _r_args args = {aq_args, T};
//     // adaptavive_quadrature_args aq_args = {-10, 10, abs_sinc, 0.00001};
//     // _r_args args = {aq_args, 4};


//     // // Sequential
//     // printf("START SEQUENTIAL********************\n");

//     // long int sequential_time = (long int) ustopwatch(sequential_routine, &args);

//     // printf("******************** END SEQUENTIAL\n");
//     // printf("Elapsed: %ld microseconds\n\n", sequential_time);

//     //OpenMP
//     printf("START OPENMP********************\n");

//     long int omp_time = (long int) ustopwatch(omp_routine, &args);

//     printf("******************** END OPENMP\n");
//     printf("Elapsed: %ld microseconds\n\n", omp_time);

//     return 0;
// }