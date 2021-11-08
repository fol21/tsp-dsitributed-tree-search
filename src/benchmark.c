#include <benchmark.h>



double stopwatch(void (*routine)(void*), void* args)
{
    struct timeval current_time;
    
    gettimeofday(&current_time, NULL);
    double tic = (double)current_time.tv_sec + current_time.tv_usec / 1000000.0;
    
    routine(args);

    gettimeofday(&current_time, NULL);
    double toc = (double)current_time.tv_sec + current_time.tv_usec / 1000000.0;

    return (toc - tic);
}

double mstopwatch(void (*routine)(void*), void* args)
{
    double seconds = stopwatch(routine, args);
    return seconds * 1000;
}

double ustopwatch(void (*routine)(void*), void* args)
{
    double seconds = stopwatch(routine, args);
    return seconds * 1000000;
}

double pstopwatch(void (*routine)(void*), void* args)
{
    double seconds = stopwatch(routine, args);
    return seconds * 1000000000;
}