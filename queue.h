#ifndef QUEUE_H
#define QUEUE_H

// queue of string structure
struct Queue
{
  int front, rear, size;
  unsigned int capacity;
  char **array;
};

struct Queue *createQueue();
int isEmpty(struct Queue *q);
int isFull(struct Queue *q);
void enqueue(struct Queue *q, char *item);
char *dequeue(struct Queue *q);
void freeQueue(struct Queue *q);
void showQueue(struct Queue *q);

#endif