#ifndef STACK_H
#define STACK_H

#define SUCCESS 1
#define NO_MEMORY 0

typedef struct s_stack_node
{
  struct s_stack_node *prev;
  char info[200];
  int number;
} t_stack_node;

typedef t_stack_node *t_stack;

void createStack(t_stack *p);
int pushStack(t_stack *p, char *d);
int popStack(t_stack *p, char *d);
int emptyStack(t_stack *p);

#endif