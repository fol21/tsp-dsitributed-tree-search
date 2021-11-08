#ifndef QUEUE_H
#define QUEUE_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>  

#define TYPE void*
  
// A structure to represent a FIFO queue
// enqueue ---> |rear|*|*|*|*|front| -----> dequeue
typedef struct Queue_v {
    int front, rear, size;
    unsigned capacity;
    void** array;
} Queue_v;
  
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue_v* createQueue(unsigned capacity);
  
// Queue_v is full when size becomes
// equal to the capacity
bool isFull(struct Queue_v* queue);
  
// Queue_v is empty when size is 0
bool isEmpty(struct Queue_v* queue);
  
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue_v* queue, void* item);

// Function to remove an item from queue.
// It changes front and size
void* dequeue(struct Queue_v* queue);

// Function to get front of queue
void* front(struct Queue_v* queue);

// Function to get rear of queue
void* rear(struct Queue_v* queue);

#endif // !QUEUE_H

// int main()
// {
//     struct Queue_v* queue = createQueue(1000);
  
//     enqueue(queue, 10);
//     enqueue(queue, 20);
//     enqueue(queue, 30);
//     enqueue(queue, 40);
  
//     printf("%d dequeued from queue\n\n",
//            dequeue(queue));
  
//     printf("Front item is %d\n", front(queue));
//     printf("Rear item is %d\n", rear(queue));
  
//     return 0;
// }