#ifndef TOUR_QUEUE_H
#define TOUR_QUEUE_H

#include<tour.h>
#include<stdbool.h>

#define MAX_QUEUE_SIZE 1024
struct tour_queue_t {
    tour_t list[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int capacity;
};

typedef struct tour_queue_t* tour_queue_t;

tour_queue_t alloc_queue();
void enqueue(tour_queue_t queue, tour_t t);
void enqueue_copy(tour_queue_t queue, tour_t t);
tour_t dequeue(tour_queue_t queue); // Out of the team
tour_t front(tour_queue_t queue); // Access the team leader element, but do not delete it
tour_t rear(tour_queue_t queue); // Access the tail element, but do not delete it
int queue_size(tour_queue_t queue);
bool queue_empty(tour_queue_t queue);

#endif // !TOUR_QUEUE_H




