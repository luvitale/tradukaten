#ifndef STACK_H
#define STACK_H

// stack structure
typedef struct Stack
{
  int *data;
  int size;
  int capacity;
  int top;
} stack_t;

stack_t *create_stack();
int stack_is_empty(stack_t *stack);
int stack_is_full(stack_t *stack);
void push_to_stack(stack_t *, int);
int pop_from_stack(stack_t *);
void free_stack(stack_t *);

#endif