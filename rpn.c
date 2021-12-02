#include "rpn.h"
#include "utils/asm_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  et_list = create_int_list();
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
  // When the cell is a JMP
  if (strstr((char *)content, "#"))
  {
    char num_cell_str[100];
    sprintf(num_cell_str, "%s", (char *)&content[1]);

    int num_cell = atoi(num_cell_str);

    add_element_to_list(et_list, num_cell);
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

void do_binary_operation(rpn_t *rpn, FILE *fp, stack_structure_t *cell_stack, char *operation, char *op1, char *op2)
{
  pop_from_asm_stack(cell_stack, op2);
  pop_from_asm_stack(cell_stack, op1);

  fprintf(
      fp,
      "FLD %s\n",
      op1);

  fprintf(
      fp,
      "FLD %s\n",
      op2);

  if (strcmp(operation, "+") == 0)
  {
    fprintf(fp, "FADD\n");
  }
  else if (strcmp(operation, "-") == 0)
  {
    fprintf(fp, "FSUB\n");
  }
  else if (strcmp(operation, "*") == 0)
  {
    fprintf(fp, "FMUL\n");
  }
  else if (strcmp(operation, "/") == 0)
  {
    fprintf(fp, "FDIV\n");
  }

  char result[100];

  strcpy(result, "@result");

  fprintf(
      fp,
      "FSTP %s\n",
      result);

  push_to_asm_stack(cell_stack, result);
}

char *get_asm_jump(char *branch)
{
  if (strcmp(branch, "BLT") == 0)
  {
    return "JNAE";
  }
  else if (strcmp(branch, "BLE") == 0)
  {
    return "JNA";
  }
  else if (strcmp(branch, "BGT") == 0)
  {
    return "JNBE";
  }
  else if (strcmp(branch, "BGE") == 0)
  {
    return "JNB";
  }
  else if (strcmp(branch, "BEQ") == 0)
  {
    return "JE";
  }
  else if (strcmp(branch, "BNE") == 0)
  {
    return "JNE";
  }
  return "";
}

void add_asm_start_code(FILE *fp)
{
  fprintf(fp, ".CODE\n");
  fprintf(fp, "START:\n");
  fprintf(fp, "MOV AX, @DATA\n");
  fprintf(fp, "MOV DS, AX\n");
  fprintf(fp, "MOV es, AX\n");
  fprintf(fp, "FINIT\n");
}

void add_asm_end_code(FILE *fp)
{
  fprintf(fp, "END START;\n");
}

void rpn_assembly(rpn_t *rpn)
{
  stack_structure_t cell_stack;
  create_asm_stack(&cell_stack, sizeof(char) * 100);
  FILE *fp = fopen("Final.asm", "w+");

  char cell[100];
  char op1[100];
  char op2[100];
  char result[100];

  add_asm_start_code(fp);

  int last_cell = get_size_of_rpn(rpn);
  int i;
  for (i = 1; i <= last_cell; ++i)
  {
    strcpy(cell, (char *)get_cell_from_rpn(rpn, i));

    int element_is_in_list = is_element_in_list(et_list, i);

    if (element_is_in_list)
    {
      fprintf(
          fp,
          "@ET%d:\n",
          i);
    }

    if (strcmp(cell, ":=") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);
      pop_from_asm_stack(&cell_stack, op2);

      fprintf(
          fp,
          "MOV %s %s\n",
          op1,
          op2);
    }
    else if (strcmp(cell, "ET") == 0)
    {
      if (!element_is_in_list)
      {
        fprintf(
            fp,
            "@ET%d:\n",
            i);
      }
    }
    else if (strstr(cell, "#") != NULL)
    {
      char asm_jump[100];
      pop_from_asm_stack(&cell_stack, asm_jump);

      char cell_num_str[100];
      sprintf(cell_num_str, "%s", &cell[1]);

      int num_cell = atoi(cell_num_str);

      fprintf(
          fp,
          "%s @ET%d\n",
          asm_jump,
          num_cell);
    }
    else if (strcmp(cell, "CMP") == 0)
    {
      pop_from_asm_stack(&cell_stack, op2);
      pop_from_asm_stack(&cell_stack, op1);

      fprintf(
          fp,
          "FLD %s\n",
          op1);

      fprintf(
          fp,
          "FCOMP %s\n",
          op2);

      fprintf(
          fp,
          "FSTSW ax\n");

      fprintf(
          fp,
          "SAHF\n");
    }
    else if (strcmp(cell, "BLT") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "BLE") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "BGT") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "BGE") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "BEQ") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "BNE") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "+") == 0 || strcmp(cell, "-") == 0 || strcmp(cell, "*") == 0 || strcmp(cell, "/") == 0)
    {
      do_binary_operation(rpn, fp, &cell_stack, cell, op1, op2);
    }
    else if (strcmp(cell, "DSP") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);

      fprintf(
          fp,
          "DSP %s\n",
          op1);
    }
    else if (strcmp(cell, "GET") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);

      fprintf(
          fp,
          "GET %s\n",
          op1);
    }
    // id || cte
    else
    {
      push_to_asm_stack(&cell_stack, cell);
    }
  }

  add_asm_end_code(fp);

  fclose(fp);
}