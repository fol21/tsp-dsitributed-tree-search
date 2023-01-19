
#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_CITIES 100

typedef struct Digraph_s {
    int num_cities;
    double** digraph;
} Digraph_s;

double abs_sinc(double x);
double carga(double x);
double offsetSin(double x);
void delay(int max_delay);
double euclidian_distance(double x1, double y1, double x2, double y2);
Digraph_s* coord_to_digraph(FILE* fp, double (*distance_func)(double, double, double, double));


#endif //  UTILS_H

