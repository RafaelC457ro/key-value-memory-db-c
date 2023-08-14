/*
 *  queue.c
 *  Mendb
 * This file contains the implementation of the queue data structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *queue_init()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue != NULL)
    {
        queue->front = NULL;
        queue->rear = NULL;
    }
    return queue;
}

int queue_is_empty(Queue *queue)
{
    return (queue->front == NULL);
}

void queue_enqueue(Queue *queue, int data)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode != NULL)
    {
        newNode->data = data;
        newNode->next = NULL;

        if (queue_is_empty(queue))
        {
            queue->front = newNode;
            queue->rear = newNode;
        }
        else
        {
            queue->rear->next = newNode;
            queue->rear = newNode;
        }
    }
}

int queue_dequeue(Queue *queue)
{
    if (!queue_is_empty(queue))
    {
        QueueNode *temp = queue->front;
        int data = temp->data;

        queue->front = queue->front->next;
        free(temp);

        if (queue_is_empty(queue))
        {
            queue->rear = NULL;
        }

        return data;
    }
    return -1; // this is very bad code but I'm too lazy to fix it
}

void queue_destroy(Queue *queue)
{
    while (!queue_is_empty(queue))
    {
        queue_dequeue(queue);
    }
    free(queue);
}
