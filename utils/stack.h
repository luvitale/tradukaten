#ifndef STACK_H
#define STACK_H

// stack structure
typedef struct Stack
{
  int *data;
  int size;
  int capacity;
  int top;
} stack_str_t;

stack_str_t *create_stack();
int stack_is_empty(stack_str_t *stack);
int stack_is_full(stack_str_t *stack);
void push_to_stack(stack_str_t *, int);
int pop_from_stack(stack_str_t *);
void free_stack(stack_str_t *);

#endif