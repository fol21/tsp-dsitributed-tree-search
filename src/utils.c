#include <utils.h>

double abs_sinc(double x)
{
    double res = x != 0 ? fabs(sin(x) / (x)) : 1.0;
    return res;
}

double carga(double x)
{

    double power[8] = {5731, 1243, 774, 736, 460, 50, 87, 126};
    double frequencies[8] = {0.11, 0.2, 0.53, 0.74, 1.1, 1.53, 2.3, 2};
    double phases[8] = {0.36, 3.5, 1.5, 1.9, -2.5, -1.36, -5, -1.2};

    double sum = 0;
    for (int i = 0; i < 8; i++)
    {
        /* code */
        sum += power[i] * sin(frequencies[i] * x + phases[i]);
    }
    return fabs(sum);
}

double offset_sin(double x)
{
    double y1 = sin((1.0/2.0) * M_PI * x);
    double y2 = cos((1.0/5.0) * M_PI * x);
    double y3 = sin((1.0/10.0) * M_PI * x);
    return y1 + y2 + y3 + 5;
}

void delay(int max_delay)
{
    sleep(rand() % max_delay);
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CITIES 100

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double euclidian_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Digraph_s* coord_to_digraph(FILE* fp, double (*distance_func)(double, double, double, double)) {
    if (fp == NULL) {
        printf("Error opening file \n");
        return NULL;
    }
    Digraph_s* ds = (Digraph_s*) malloc(sizeof(Digraph_s));
    fscanf(fp, "%d", &(ds->num_cities));
    if (ds->num_cities > MAX_CITIES) {
        printf("Error: number of cities exceeds maximum limit of %d\n", MAX_CITIES);
        return NULL;
    }
    double x[MAX_CITIES], y[MAX_CITIES];
    for (int i = 0; i < ds->num_cities; i++) {
        fscanf(fp, "%lf %lf", &x[i], &y[i]);
    }

    ds->digraph = (double**) malloc(ds->num_cities * sizeof(double*));
    for (int i = 0; i < ds->num_cities; i++) {
        (ds->digraph)[i] = (double*) malloc(ds->num_cities * sizeof(double));
    }
    for (int i = 0; i < ds->num_cities; i++) {
        for (int j = 0; j < ds->num_cities; j++) {
            (ds->digraph)[i][j] = (*distance_func)(x[i], y[i], x[j], y[j]);
        }
    }
    fclose(fp);
    return ds;
}
