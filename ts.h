#ifndef TS_H
#define TS_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

enum type
{
  not_type = 0,
  integer,
  constant_int,
  real,
  constant_real,
  string,
  constant_str,
  identifier,
  constant,
};

typedef struct
{
  char lexeme[100];
  enum type datatype;
  char value[100];
  char length[4];
  int isStored;
} t_symbol_row;

t_symbol_row symbol_table[500];

void saveFileTS(void);
int loadToTS(char *, enum type);
void assignConstantValue(void);

#endif