#ifndef __MENDB_QUEUE_H
#define __MENDB_QUEUE_H

// Define the structure for the queue node
typedef struct QueueNode
{
    int data;
    struct QueueNode *next;
} QueueNode;

// Define the queue structure
typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

// Function prototypes
Queue *queue_init();
int queue_is_empty(Queue *queue);
void queue_enqueue(Queue *queue, int data);
int queue_dequeue(Queue *queue);
void queue_destroy(Queue *queue);

#endif /* _QUEUE_H */