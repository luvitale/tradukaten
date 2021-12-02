// C program for array implementation of queue
#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 10

// create a queue of string
queue_str_t *create_queue()
{
  queue_str_t *queue = (queue_str_t *)malloc(sizeof(queue_str_t));
  queue->capacity = INITIAL_CAPACITY;
  queue->front = queue->size = 0;
  queue->rear = queue->capacity - 1;
  queue->array = (char **)malloc(queue->capacity * sizeof(char *));
  return queue;
}

// check if the queue is empty
int queue_is_empty(queue_str_t *queue)
{
  return queue->size == 0;
}

// check if the queue is full
int queue_is_full(queue_str_t *queue)
{
  return queue->size == queue->capacity;
}

// enqueue a string into the queue
void enqueue(queue_str_t *queue, char *item)
{
  if (queue_is_full(queue))
  {
    queue->capacity *= 2;
    queue->array = (char **)realloc(queue->array, queue->capacity * sizeof(char *));
    if (queue->rear >= queue->front)
      queue->rear = (queue->rear + 1) % queue->capacity;
    else
      queue->front = (queue->front + 1) % queue->capacity;
  }
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
  //printf("%s enqueued to queue\n", item);
}

// dequeue a string from the queue
char *dequeue(queue_str_t *queue)
{
  if (queue_is_empty(queue))
    return NULL;
  char *item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  //printf("%s dequeued from queue\n", item);
  return item;
}

// get the front item from the queue
char *front(queue_str_t *queue)
{
  if (queue_is_empty(queue))
    return NULL;
  return queue->array[queue->front];
}

// get the rear item from the queue
char *rear(queue_str_t *queue)
{
  if (queue_is_empty(queue))
    return NULL;
  return queue->array[queue->rear];
}

// free the queue
void free_queue(queue_str_t *queue)
{
  free(queue->array);
  free(queue);
}

// show full queue
void show_queue(queue_str_t *queue)
{
  if (queue_is_empty(queue))
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