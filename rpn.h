#ifndef RPN_H
#define RPN_H

// lexeme string
typedef char lexeme[100];

// Reverse Polish Notation
typedef struct
{
  int size;
  int capacity;
  lexeme **data;
} t_rpn;

t_rpn *create_rpn(int);
lexeme *get_lexeme_from_rpn(t_rpn *, int);
void set_lexeme_from_rpn(t_rpn *, int, lexeme *);
void add_lexeme_to_rpn(t_rpn *, lexeme *);
void save_lexemes_in_file(t_rpn *);
void free_rpn(t_rpn *);

#endif