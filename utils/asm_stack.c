#include "asm_stack.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Create stack
 * 
 */
void create_asm_stack(stack_structure_t *s, int elem_size)
{
  s->node = NULL;
  s->elem_size = elem_size;
}

/**
 * @brief Check if stack is empty
 * 
 * @return int 
 */
int asm_stack_is_empty(stack_structure_t *s)
{
  return s->node == NULL;
}

/**
 * @brief Push element to stack
 * 
 */
void push_to_asm_stack(stack_structure_t *s, void *value)
{
  stack_node_t *new_node = (stack_node_t *)malloc(sizeof(stack_node_t));
  if (!new_node)
  {
    fprintf(stderr, "Insufficient memory to push element to stack.\n");
    exit(EXIT_FAILURE);
  }
  new_node->next = s->node;
  new_node->value = (byte *)malloc(s->elem_size);
  if (!new_node->value)
  {
    fprintf(stderr, "Insufficient memory to push element to stack.\n");
    exit(EXIT_FAILURE);
  }
  memcpy(new_node->value, value, s->elem_size);
  s->node = new_node;
}

/**
 * @brief Pop element from stack
 * 
 */
void pop_from_asm_stack(stack_structure_t *s, void *value)
{
  if (asm_stack_is_empty(s))
  {
    fprintf(stderr, "Stack is empty.\n");
    exit(EXIT_FAILURE);
  }
  stack_node_t *old_node = s->node;
  s->node = old_node->next;
  memcpy(value, old_node->value, s->elem_size);
  free(old_node->value);
  free(old_node);
}

/**
 * @brief Get top element from stack
 * 
 */
void top_from_asm_stack(stack_structure_t *s, void *value)
{
  if (asm_stack_is_empty(s))
  {
    fprintf(stderr, "Stack is empty.\n");
    exit(EXIT_FAILURE);
  }
  memcpy(value, s->node->value, s->elem_size);
}

/**
 * @brief Destroy stack
 * 
 */
void destroy_asm_stack(stack_structure_t *s)
{
  while (!asm_stack_is_empty(s))
  {
    pop_from_asm_stack(s, NULL);
  }
  free(s->node);
  s->node = NULL;
}