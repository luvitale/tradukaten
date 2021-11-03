#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>

// create reverse polish notacion
t_rpn *create_rpn(int capacity)
{
  t_rpn *array = malloc(sizeof(t_rpn));
  array->size = 0;
  array->capacity = capacity;
  array->data = malloc(sizeof(lexeme) * capacity);
  return array;
}

// get element from rpn in position num
lexeme *get_lexeme_from_rpn(t_rpn *arr, int num)
{
  return *(arr->data + num);
}

// set element from rpn in position num
void set_lexeme_from_rpn(t_rpn *arr, int num, lexeme *lex)
{
  arr->data[num] = lex;
}

// add element to rpn
void add_lexeme_to_rpn(t_rpn *arr, lexeme *lex)
{
  if (arr->size == arr->capacity)
  {
    arr->capacity *= 2;
    arr->data = realloc(arr->data, sizeof(lexeme) * arr->capacity);
  }
  arr->data[arr->size] = lex;
  arr->size++;
}

// save lexemes in file
void save_lexemes_in_file(t_rpn *arr)
{
  FILE *file = fopen("intermedia.txt", "w");
  for (int i = 0; i < arr->size; i++)
  {
    fprintf(file, "%s\n", (char *)get_lexeme_from_rpn(arr, i));
  }
  fclose(file);
}

// free array of lexemes
void free_rpn(t_rpn *arr)
{
  free(arr->data);
  free(arr);
}