#include<tour.h>

tour_t alloc_tour()
{
    tour_t tour = malloc(sizeof(struct tour_t));

    tour->len = 0;
    tour->cost = 0;
    return tour;
}

// Copy tour;
void copy_tour(tour_t dest, tour_t src)
{
    dest->len = src->len;
    dest->cost = src->cost;

    for (int i = 0; i < src->len; ++i) {
        dest->tour[i] = src->tour[i];
    }
}

void output_tour(tour_t t)
{
    int i;

    if (t->len <= 0) {
        printf("Empty\n");
        return;
    }

    for (i = 0; i < t->len - 1; ++i) {
        printf("%d -> ", t->tour[i]);
    }

    printf("%d\n", t->tour[i]);
}

int get_last_city(tour_t t)
{
    return t->tour[t->len - 1];
}

void append_city(tour_t t, int city, int digraph_value)
{
    t->cost += digraph_value;
    // t->cost += digraph[get_last_city(t)][city];
    t->tour[t->len++] = city;
}

int remove_last_city(tour_t t, int digraph_value)
{
    t->cost -= digraph_value;
    // t->cost -= digraph[t->tour[t->len - 2]][t->tour[t->len - 1]];
    return t->tour[--t->len];
}

int find_in_tour(tour_t t, int city)
{
    for (int i = 0; i < t->len; ++i) {
        if (t->tour[i] == city) {
            return i;
        }
    }

    return -1;
}