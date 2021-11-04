#ifndef RPN_H
#define RPN_H

// lexeme string
typedef char lexeme_t[100];

// Reverse Polish Notation
typedef struct RPN
{
  int size;
  int capacity;
  lexeme_t **lex;
} rpn_t;

rpn_t *create_rpn();
lexeme_t *get_lexeme_from_rpn(rpn_t *, int);
void set_lexeme_from_rpn(rpn_t *, int, lexeme_t *);
void add_lexeme_to_rpn(rpn_t *, lexeme_t *);
int get_actual_cell_from_rpn(rpn_t *);
void save_lexemes_in_file(rpn_t *);
void free_rpn(rpn_t *);

#endif