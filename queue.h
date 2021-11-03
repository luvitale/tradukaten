#ifndef QUEUE_H
#define QUEUE_H

// queue of string structure
struct Queue
{
  int front, rear, size;
  unsigned int capacity;
  char **array;
};

struct Queue *create_queue();
int is_empty(struct Queue *q);
int is_full(struct Queue *q);
void enqueue(struct Queue *q, char *item);
char *dequeue(struct Queue *q);
void free_queue(struct Queue *q);
void show_queue(struct Queue *q);

#endif