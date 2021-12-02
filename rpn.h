#ifndef RPN_H
#define RPN_H

// lexeme string
typedef char cell_t[100];

// Reverse Polish Notation
typedef struct RPN
{
  int size;
  int capacity;
  cell_t **cell;
} rpn_t;

rpn_t *create_rpn();
cell_t *get_cell_from_rpn(rpn_t *, int);
void set_cell_from_rpn(rpn_t *, int, cell_t *);
void add_cell_to_rpn(rpn_t *, cell_t *);
int get_actual_cell_from_rpn(rpn_t *);
void save_cells_in_file(rpn_t *);
void free_rpn(rpn_t *);

#endif