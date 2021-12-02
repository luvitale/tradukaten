#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 10
#define FILENAME "intermedia.txt"

/**
 * @brief Create Reverse Polish Notacion
 * 
 * @return rpn_t* 
 */
rpn_t *create_rpn()
{
  rpn_t *rpn = malloc(sizeof(rpn_t));
  rpn->size = 0;
  rpn->capacity = INITIAL_CAPACITY;
  rpn->cell = malloc(sizeof(cell_t) * rpn->capacity);
  return rpn;
}

/**
 * @brief Get cell from RPN
 * 
 * @param rpn 
 * @param num_cell 
 * @return cell_t* 
 */
cell_t *get_cell_from_rpn(rpn_t *rpn, int num_cell)
{
  int pos = num_cell - 1;
  return *(rpn->cell + pos);
}

/**
 * @brief Set cell from RPN
 * 
 * @param rpn 
 * @param num_cell 
 * @param content 
 */
void set_cell_from_rpn(rpn_t *rpn, int num_cell, cell_t *content)
{
  int pos = num_cell - 1;
  rpn->cell[pos] = content;
}

/**
 * @brief Add cell to RPN
 * 
 * @param rpn 
 * @param content 
 */
void add_cell_to_rpn(rpn_t *rpn, cell_t *content)
{
  if (rpn->size == rpn->capacity)
  {
    rpn->capacity *= 2;
    rpn->cell = realloc(rpn->cell, sizeof(cell_t) * rpn->capacity);
  }
  rpn->cell[rpn->size] = content;
  rpn->size++;
}

/**
 * @brief Get last cell from RPN
 * 
 * @param rpn 
 * @return int 
 */
cell_t *get_last_cell_from_rpn(rpn_t *rpn)
{
  return rpn->cell[rpn->size - 1];
}

/**
 * @brief Get size of RPN
 * 
 * @param rpn 
 * @return int - Quantity of cells
 */
int get_size_of_rpn(rpn_t *rpn)
{
  return rpn->size;
}

/**
 * @brief Save cells in file
 * 
 * @param rpn 
 */
void save_cells_in_file(rpn_t *rpn)
{
  FILE *file = fopen(FILENAME, "w");
  for (int num_cell = 1; num_cell <= rpn->size; ++num_cell)
  {
    fprintf(file, "%s\n", (char *)get_cell_from_rpn(rpn, num_cell));
  }
  fclose(file);
}

/**
 * @brief Free RPN
 * 
 * @param rpn 
 */
void free_rpn(rpn_t *rpn)
{
  free(rpn->cell);
  free(rpn);
}