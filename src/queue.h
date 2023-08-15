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