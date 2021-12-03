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

void do_binary_operation(rpn_t *rpn, table_t *symbol_table, FILE *fp, stack_structure_t *cell_stack, char *operation, int num_cell)
{
  char op1[100];
  char op2[100];

  pop_from_asm_stack(cell_stack, op2);
  pop_from_asm_stack(cell_stack, op1);

  fprintf(
      fp,
      "\tFLD %s\n",
      op1);

  fprintf(
      fp,
      "\tFLD %s\n",
      op2);

  if (strcmp(operation, "+") == 0)
  {
    fprintf(fp, "\tFADD ; Add\n");
  }
  else if (strcmp(operation, "-") == 0)
  {
    fprintf(fp, "\tFSUB ; Substraction\n");
  }
  else if (strcmp(operation, "*") == 0)
  {
    fprintf(fp, "\tFMUL ; Multiplication\n");
  }
  else if (strcmp(operation, "/") == 0)
  {
    fprintf(fp, "\tFDIV ; Division\n");
  }

  char result[100];

  sprintf(result, "@result_%d", num_cell);

  fprintf(
      fp,
      "\tFSTP %s ; Assign result of operation\n",
      result);

  fprintf(fp, "\n");

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
  else if (strcmp(branch, "BI") == 0)
  {
    return "JMP";
  }
  else
  {
    return "";
  }
}

char *get_asm_comment(char *branch)
{
  if (strcmp(branch, "BLT") == 0)
  {
    return "Jump if not above or equal";
  }
  else if (strcmp(branch, "BLE") == 0)
  {
    return "Jump if not above";
  }
  else if (strcmp(branch, "BGT") == 0)
  {
    return "Jump if not below or equal";
  }
  else if (strcmp(branch, "BGE") == 0)
  {
    return "Jump if not below";
  }
  else if (strcmp(branch, "BEQ") == 0)
  {
    return "Jump if equal";
  }
  else if (strcmp(branch, "BNE") == 0)
  {
    return "Jump if not equal";
  }
  else if (strcmp(branch, "BI") == 0)
  {
    return "Inconditional jump";
  }
  else
  {
    return "";
  }
}

void add_asm_includes(FILE *fp)
{
  fprintf(fp, "include macros2.asm\n");
  fprintf(fp, "include number.asm\n");
  fprintf(fp, "\n");
}

void add_asm_header(FILE *fp)
{
  add_asm_includes(fp);
  fprintf(fp, ".MODEL LARGE\n");
  fprintf(fp, ".386\n");
  fprintf(fp, ".STACK 200h\n");
  fprintf(fp, "\n");
}

void add_asm_data(FILE *fp, table_t *symbol_table)
{
  fprintf(fp, ".DATA\n");
  fprintf(fp, "; Table of symbols\n");

  node_t *symbol = *symbol_table;

  while (symbol)
  {
    char str_length[20];
    char str_value[102];

    sprintf(str_length, ", \'$\', %d dup (?)", symbol->datatype == constant_str ? symbol->length : 100);
    sprintf(str_value, "\"%s\"", symbol->value);

    fprintf(fp,
            "%-40s %-5s %-102s %s\n",
            symbol->name,
            symbol->datatype == constant_str || symbol->datatype == str ? "db" : "dd",
            symbol->datatype == integer || symbol->datatype == real ?
              "?" :
              symbol->datatype == str ?
                "\"\"" : symbol->datatype == constant_str ?
                  str_value : symbol->value,
            symbol->datatype == constant_str || symbol->datatype == str ? str_length : "");
    symbol = symbol->next;
  }

  fprintf(fp, "\n");
}

void add_asm_start_code(FILE *fp)
{
  fprintf(fp, ".CODE\n");
  fprintf(fp, "\n");

  fprintf(fp, "START:\n");
  fprintf(fp, "\tMOV AX, @DATA\n");
  fprintf(fp, "\tMOV DS, AX\n");
  fprintf(fp, "\tMOV es, AX\n");
  fprintf(fp, "\tFINIT ; Initialize coprocessor\n");
  fprintf(fp, "\n");
}

void add_asm_end_code(FILE *fp)
{
  fprintf(fp, "FINAL:\n");
  fprintf(fp, "\tMOV AX, 4C00h ; Exit\n");
  fprintf(fp, "\tINT 21h ; Send interrupt\n");
  fprintf(fp, "\tFFREE\n");
  fprintf(fp, "\tstrlen proc near\n");
  fprintf(fp, "\t\tmov bx, 0\n");
  fprintf(fp, "\t\t@strl01:\n");
  fprintf(fp, "\t\tcmp BYTE PTR [si+bx],'$'\n");
  fprintf(fp, "\t\tje @strend\n");
  fprintf(fp, "\t\tinc bx\n");
  fprintf(fp, "\t\tjmp @strl01\n");
  fprintf(fp, "\t\t@strend:\n");
  fprintf(fp, "\t\tret\n");
  fprintf(fp, "\tstrlen endp\n");

  fprintf(fp, "\tstrcpy proc near\n");
  fprintf(fp, "\t\tcall strlen\n");
  fprintf(fp, "\t\tcmp bx , 100\n");
  fprintf(fp, "\t\tjle @ok_size_copy\n");
  fprintf(fp, "\t\tmov bx , 100\n");
  fprintf(fp, "\t\t@ok_size_copy:\n");
  fprintf(fp, "\t\tmov cx , bx\n");
  fprintf(fp, "\t\tcld\n");
  fprintf(fp, "\t\trep movsb\n");
  fprintf(fp, "\t\tmov al , '$'\n");
  fprintf(fp, "\t\tmov byte ptr[di],al\n");
  fprintf(fp, "\t\tret\n");
  fprintf(fp, "\tstrcpy endp\n\n");
  fprintf(fp, "END START ;");
}

void rpn_assemble(rpn_t *rpn, table_t *symbol_table, int_list_t *et_list)
{
  stack_structure_t cell_stack;
  create_asm_stack(&cell_stack, sizeof(char) * 100);
  FILE *fp = fopen("Final.asm", "w+");

  char cell[100];
  char op1[100];
  char op2[100];
  char result[100];

  add_asm_header(fp);

  add_asm_data(fp, symbol_table);

  add_asm_start_code(fp);

  int last_cell = get_size_of_rpn(rpn);
  int i;
  for (i = 1; i <= last_cell; ++i)
  {
    strcpy(cell, (char *)get_cell_from_rpn(rpn, i));

    int element_is_in_list = is_element_in_list(et_list, i);

    if (element_is_in_list)
    {
      fprintf(fp, "\n");

      fprintf(
          fp,
          "@ET%d:\n",
          i);
    }

    if (strcmp(cell, ":=") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);
      pop_from_asm_stack(&cell_stack, op2);

      if (strstr(op2, "_s_") != NULL) {
        fprintf(fp, "\t; Assign string to variable\n");
        fprintf(fp, "\tMOV SI, OFFSET %s\n", op2);
        fprintf(fp, "\tMOV DI, OFFSET %s\n", op1);
        fprintf(fp, "\tCALL strcpy\n");
        fprintf(fp, "\n");
      }
      else {
        fprintf(fp, "\t; Assign to variable\n");

        fprintf(
          fp,
          "\tFLD %s\n",
          op2);

        fprintf(
            fp,
            "\tFSTP %s\n",
            op1);

        fprintf(fp, "\n");
      }
    }
    else if (strcmp(cell, "ET") == 0)
    {
      if (!element_is_in_list)
      {
        fprintf(fp, "\n");

        fprintf(
            fp,
            "@ET%d:\n",
            i);
      }
    }
    else if (strstr(cell, "#") != NULL)
    {
      char cell_content[100];
      char *asm_jump;
      char *asm_comment;
      pop_from_asm_stack(&cell_stack, cell_content);

      asm_jump = get_asm_jump(cell_content);
      asm_comment = get_asm_comment(cell_content);

      char cell_num_str[100];
      sprintf(cell_num_str, "%s", &cell[1]);

      int num_cell = atoi(cell_num_str);

      fprintf(
          fp,
          "\t%s @ET%d ; %s\n",
          asm_jump,
          num_cell,
          asm_comment);
      
      fprintf(fp, "\n");
    }
    else if (strcmp(cell, "CMP") == 0)
    {
      pop_from_asm_stack(&cell_stack, op2);
      pop_from_asm_stack(&cell_stack, op1);

      fprintf(
          fp,
          "\t; Compare\n");

      fprintf(
          fp,
          "\tFLD %s\n",
          op1);

      fprintf(
          fp,
          "\tFCOMP %s\n",
          op2);

      fprintf(
          fp,
          "\tFSTSW ax\n");

      fprintf(
          fp,
          "\tSAHF\n");
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
    else if (strcmp(cell, "BI") == 0)
    {
      push_to_asm_stack(&cell_stack, cell);
    }
    else if (strcmp(cell, "+") == 0 || strcmp(cell, "-") == 0 || strcmp(cell, "*") == 0 || strcmp(cell, "/") == 0)
    {
      do_binary_operation(rpn, symbol_table, fp, &cell_stack, cell, i);
    }
    else if (strcmp(cell, "DSP") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);

      type_t cell_type = get_lex_type(symbol_table, op1);

      if (cell_type == integer || cell_type == real || cell_type == constant_int || cell_type == constant_real)
      {
        fprintf(
            fp,
            "\tdisplayFloat %s ; Print number\n",
            op1);
      }
      else if (strcmp(op1, "_s_") == 0) {
        fprintf(fp, "\tnewLine\n", op1);
      }
      else {
        fprintf(
            fp,
            "\tdisplayString %s ; Print string\n",
            op1);
      }
    }
    else if (strcmp(cell, "GET") == 0)
    {
      pop_from_asm_stack(&cell_stack, op1);

      type_t cell_type = get_lex_type(symbol_table, op1);

      if (cell_type == integer || cell_type == real || cell_type == constant_int || cell_type == constant_real)
      {
        fprintf(
            fp,
            "\tgetFloat %s ; Get number\n",
            op1);
      }
      else {
        fprintf(
            fp,
            "\tgetString %s ; Get string\n",
            op1);
      }
    }
    // id || cte
    else
    {
      push_to_asm_stack(&cell_stack, cell);
    }
  }

  fprintf(fp, "\n");
  fprintf(fp, "; End\n");
  fprintf(fp, "@ET%d:\n", i);
  fprintf(fp, "\n");

  add_asm_end_code(fp);

  fclose(fp);
}