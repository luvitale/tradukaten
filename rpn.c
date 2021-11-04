#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 10
#define FILENAME "intermedia.txt"

// create reverse polish notacion
rpn_t *create_rpn()
{
  rpn_t *rpn = malloc(sizeof(rpn_t));
  rpn->size = 0;
  rpn->capacity = INITIAL_CAPACITY;
  rpn->lex = malloc(sizeof(lexeme_t) * rpn->capacity);
  return rpn;
}

// get element from rpn in cell
lexeme_t *get_lexeme_from_rpn(rpn_t *rpn, int cell)
{
  return *(rpn->lex + cell);
}

// set element from rpn in cell
void set_lexeme_from_rpn(rpn_t *rpn, int cell, lexeme_t *lex)
{
  rpn->lex[cell - 1] = lex;
}

// add element to rpn
void add_lexeme_to_rpn(rpn_t *rpn, lexeme_t *lex)
{
  if (rpn->size == rpn->capacity)
  {
    rpn->capacity *= 2;
    rpn->lex = realloc(rpn->lex, sizeof(lexeme_t) * rpn->capacity);
  }
  rpn->lex[rpn->size] = lex;
  rpn->size++;
}

// get actual field from rpn
int get_actual_cell_from_rpn(rpn_t *rpn)
{
  return rpn->size;
}

// save lexemes in file
void save_lexemes_in_file(rpn_t *rpn)
{
  FILE *file = fopen(FILENAME, "w");
  for (int i = 0; i < rpn->size; i++)
  {
    fprintf(file, "%s\n", (char *)get_lexeme_from_rpn(rpn, i));
  }
  fclose(file);
}

// free array of lexemes
void free_rpn(rpn_t *rpn)
{
  free(rpn->lex);
  free(rpn);
}