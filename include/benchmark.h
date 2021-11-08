#ifndef BENCHMARK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>


double stopwatch(void (*routine)(void*), void* args);
double mstopwatch(void (*routine)(void*), void* args);
double ustopwatch(void (*routine)(void*), void* args);
double pstopwatch(void (*routine)(void*), void* args);

#endif // !BENCHMARK_H