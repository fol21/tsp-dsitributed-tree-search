#include <queue.h>
  
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue_v* createQueue(unsigned capacity)
{
    struct Queue_v* queue = (struct Queue_v*) malloc(sizeof(struct Queue_v));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
  
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (void**) malloc(queue->capacity * sizeof(void*));
    return queue;
}
  
// Queue_v is full when size becomes
// equal to the capacity
bool isFull(struct Queue_v* queue)
{
    return (queue->size == queue->capacity);
}
  
// Queue_v is empty when size is 0
bool isEmpty(struct Queue_v* queue)
{
    return (queue->size == 0);
}
  
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue_v* queue, void* item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    // printf("%d enqueued to queue\n", item);
}
  
// Function to remove an item from queue.
// It changes front and size
void* dequeue(struct Queue_v* queue)
{
    if (isEmpty(queue))
        return NULL;
    void* item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
  
// Function to get front of queue
void* front(struct Queue_v* queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}
  
// Function to get rear of queue
void* rear(struct Queue_v* queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->rear];
}
  
