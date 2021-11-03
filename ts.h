#ifndef TS_H
#define TS_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef enum type
{
  constant_int = 0,
  constant_real,
  constant_str,
  integer,
  real,
  str,
} t_type;

#define SUCCESS 1
#define DUPLICATED 2
#define NO_MEMORY 0

typedef struct s_node
{
  char name[50];
  enum type datatype;
  char value[50];
  int length;
  struct s_node *next;
} t_node;

// get string from enum type
char *get_string_type(t_type type);

// get enum type from string
t_type get_enum_type(const char *type_str);

typedef t_node *t_list;

void create_list(t_list *p);
int insert_order(t_list *p, char *name, t_type datatype, char *value, int length);
int insert_variable(t_list *p, char *name, t_type datatype);
int insert_string(t_list *p, char *name);
int insert_integer(t_list *p, int lex);
int insert_real(t_list *p, double lex);
void save_table_in_file(t_list *p);

#endif