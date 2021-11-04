#ifndef QUEUE_H
#define QUEUE_H

// queue of string structure
typedef struct Queue
{
  int front, rear, size;
  unsigned int capacity;
  char **array;
} queue_t;

queue_t *create_queue();
int queue_is_empty(queue_t *);
int queue_is_full(queue_t *);
void enqueue(queue_t *, char *);
char *dequeue(queue_t *);
void free_queue(queue_t *);
void show_queue(queue_t *);

#endif