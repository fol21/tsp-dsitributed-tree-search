#ifndef TOUR_H
#define TOUR_H

#include<stdlib.h>
#include<stdio.h>

#define MAX_CITY_NUM 100

struct tour_t {
    int tour[MAX_CITY_NUM];
    int len;
    double cost;
};

typedef struct tour_t* tour_t;

// Apply for a tour node
tour_t create_tour();
void copy_tour(tour_t dest, tour_t src);
void output_tour(tour_t t);

int get_last_city(tour_t t);
void append_city(tour_t t, int city, double digraph_value);
int remove_last_city(tour_t t, double digraph_value);
int find_in_tour(tour_t t, int city);

#endif // !TOUR_H