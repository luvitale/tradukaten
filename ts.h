#ifndef TS_H
#define TS_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

enum type
{
  constant_int = 0,
  constant_real,
  constant_str,
  integer,
  real,
  str,
};

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
char *getStringType(enum type type);

// get enum type from string
enum type getEnumType(const char *type_str);

typedef t_node *t_list;

void createList(t_list *p);
int insertOrder(t_list *p, char *name, enum type datatype, char *value, int length);
int insertVariable(t_list *p, char *name, enum type datatype);
int insertString(t_list *p, char *name);
int insertInteger(t_list *p, int lex);
int insertReal(t_list *p, double lex);
char *deleteQuotes(char *lex);
void saveTableInFile(t_list *p);

#endif