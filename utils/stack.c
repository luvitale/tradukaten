#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 10

// create stack
stack_str_t *create_stack()
{
  stack_str_t *stack = malloc(sizeof(stack_str_t));
  stack->capacity = INITIAL_CAPACITY;
  stack->size = 0;
  stack->data = malloc(sizeof(void *) * stack->capacity);
  return stack;
}

// check if stack is full
int stack_is_full(stack_str_t *stack)
{
  return stack->size == stack->capacity;
}

// check if stack is empty
int stack_is_empty(stack_str_t *stack)
{
  return stack->size == 0;
}

// push element to stack
void push_to_stack(stack_str_t *stack, int value)
{
  if (stack_is_full(stack))
  {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, sizeof(void *) * stack->capacity);
  }
  stack->data[stack->size] = value;
  stack->size++;
}

// pop element from stack
int pop_from_stack(stack_str_t *stack)
{
  if (stack_is_empty(stack))
  {
    printf("Stack is empty\n");
    return -1;
  }
  stack->size--;
  return stack->data[stack->size];
}

// free stack
void free_stack(stack_str_t *stack)
{
  free(stack->data);
  free(stack);
}