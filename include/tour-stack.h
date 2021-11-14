#ifndef TOUR_STACK_H
#define TOUR_STACK_H

#include<tour.h>
#include<stdbool.h>

#define MAX_STACK_SIZE 2048

struct tour_stack_t {
    tour_t list[MAX_STACK_SIZE];
    int top;
};

typedef struct tour_stack_t* tour_stack_t;

tour_stack_t alloc_stack();
void push(tour_stack_t stack, tour_t t);
void push_copy(tour_stack_t stack, tour_t t);
tour_t top(tour_stack_t stack);
tour_t pop(tour_stack_t stack);
bool stack_empty(tour_stack_t stack);
int stack_size(tour_stack_t stack);


#endif // !TOUR_STACK_H