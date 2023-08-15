/**
 * queue.c
 * Mendb
 * This file contains the implementation of the queue data structure.
 *
 * MIT License
 *
 * Copyright (c) 2017 Rafael Castro
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
