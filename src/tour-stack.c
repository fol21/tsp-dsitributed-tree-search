#include<tour-stack.h>

tour_stack_t alloc_stack()
{
    tour_stack_t stack = malloc(sizeof(struct tour_stack_t));

    stack->top = -1;

    return stack;
}



void push(tour_stack_t stack, tour_t t)
{
    stack->list[++stack->top] = t;
}

void push_copy(tour_stack_t stack, tour_t t)
{
    tour_t tmp = alloc_tour();

    copy_tour(tmp, t);
    push(stack, tmp);
}

tour_t top(tour_stack_t stack)
{
    return stack->list[stack->top];
}

tour_t pop(tour_stack_t stack)
{
    return stack->list[stack->top--];
}

int stack_size(tour_stack_t stack)
{
    return stack->top + 1;
}

bool stack_empty(tour_stack_t stack)
{
    return stack_size(stack) == 0;
}