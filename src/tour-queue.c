#include<tour-queue.h>

tour_queue_t create_queue()
{
    tour_queue_t queue = malloc(sizeof(struct tour_queue_t));

    queue->capacity = sizeof(queue->list);
    queue->front = queue->rear = 0;

    return queue;
}

void enqueue(tour_queue_t queue, tour_t t)
{
    queue->list[queue->rear] = t;
    queue->rear = (queue->rear + 1) % queue->capacity;
}

void enqueue_copy(tour_queue_t queue, tour_t t)
{
    tour_t tmp = create_tour();
    copy_tour(tmp, t);
    enqueue(queue, tmp);
}

tour_t dequeue(tour_queue_t queue)
{
    tour_t tmp = front(queue);
    queue->front = (queue->front + 1) % queue->capacity;
    return tmp;
}

tour_t front(tour_queue_t queue)
{
    return queue->list[queue->front];
}

tour_t rear(tour_queue_t queue)
{
    return queue->list[(queue->rear - 1) % queue->capacity];
}

int queue_size(tour_queue_t queue)
{
    return (queue->rear - queue->front + queue->capacity) % queue->capacity;
}

bool queue_empty(tour_queue_t queue)
{
    return queue->front == queue->rear;
}