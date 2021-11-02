#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void createStack(t_stack *p)
{
  *p = NULL;
}

int pushStack(t_stack *p, char *info)
{

  t_stack_node *new_node = (t_stack_node *)malloc(sizeof(t_stack_node));

  if (!new_node)
    return NO_MEMORY;

  strcpy(new_node->info, info);

  new_node->prev = *p;
  *p = new_node;

  return SUCCESS;
}

int popStack(t_stack *p, char *info)
{
  t_stack_node *old_node;
  if (!*p)
    return NO_MEMORY;
  old_node = (t_stack_node *)malloc(sizeof(t_stack_node));
  old_node = *p;
  strcpy(info, old_node->info);
  *p = old_node->prev;
  free(old_node);
  return SUCCESS;
}

int emptyStack(t_stack *p)
{
  return !*p;
}