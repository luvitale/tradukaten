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
} type_t;

#define SUCCESS 1
#define DUPLICATED 2
#define NO_MEMORY 0

typedef struct Node
{
  char name[50];
  type_t datatype;
  char value[50];
  int length;
  struct Node *next;
} node_t;

// get string from enum type
char *get_string_type(type_t type);

// get enum type from string
type_t get_enum_type(const char *type_str);

typedef node_t *table_t;

void create_list(table_t *p);

type_t get_lex_type(table_t *, char *);

int insert_order(table_t *p, char *name, type_t datatype, char *value, int length);
int insert_variable(table_t *p, char *name, type_t datatype);
int insert_string(table_t *p, char *name);
int insert_integer(table_t *p, int lex);
int insert_real(table_t *p, double lex);
char *get_string_name(char *);
void save_table_in_file(table_t *p);
void delete_list(table_t *p);

#endif