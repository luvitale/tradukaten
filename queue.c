// C program for array implementation of queue
#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// create a queue of string
struct Queue *create_queue(int capacity)
{
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));

  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (char **)malloc(queue->capacity * sizeof(char *));
  return queue;
}

// check if the queue is empty
int is_empty(struct Queue *queue)
{
  return queue->size == 0;
}

// check if the queue is full
int is_full(struct Queue *queue)
{
  return queue->size == queue->capacity;
}

// enqueue a string into the queue
void enqueue(struct Queue *queue, char *item)
{
  if (is_full(queue))
    return;
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
  //printf("%s enqueued to queue\n", item);
}

// dequeue a string from the queue
char *dequeue(struct Queue *queue)
{
  if (is_empty(queue))
    return NULL;
  char *item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  //printf("%s dequeued from queue\n", item);
  return item;
}

// get the front item from the queue
char *front(struct Queue *queue)
{
  if (is_empty(queue))
    return NULL;
  return queue->array[queue->front];
}

// get the rear item from the queue
char *rear(struct Queue *queue)
{
  if (is_empty(queue))
    return NULL;
  return queue->array[queue->rear];
}

// free the queue
void free_queue(struct Queue *queue)
{
  free(queue->array);
  free(queue);
}

// show full queue
void show_queue(struct Queue *queue)
{
  if (is_empty(queue))
  {
    printf("Queue is empty\n");
    return;
  }
  int i = queue->front;
  for (int j = 0; j < queue->size; j++)
  {
    printf("%s ", queue->array[i]);
    i = (i + 1) % queue->capacity;
  }
  printf("\n");
}