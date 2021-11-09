#ifndef QUEUE_H
#define QUEUE_H

// queue of string structure
typedef struct Queue
{
  int front, rear, size;
  unsigned int capacity;
  char **array;
} queue_str_t;

queue_str_t *create_queue();
int queue_is_empty(queue_str_t *);
int queue_is_full(queue_str_t *);
void enqueue(queue_str_t *, char *);
char *dequeue(queue_str_t *);
void free_queue(queue_str_t *);
void show_queue(queue_str_t *);

#endif