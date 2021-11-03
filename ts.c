#include "ts.h"

const char *const type_str[] = {
    [constant_int] = "Constant Integer",
    [constant_real] = "Constant Real",
    [constant_str] = "Constant String",
    [integer] = "ID Integer",
    [real] = "ID Real",
    [str] = "ID String",
};

// get type size of the enum type
const int type_size = sizeof(type_str) / sizeof(type_str[0]);

// get string from enum type
char *get_string_type(t_type t)
{
  return (char *)type_str[t];
}

// get enum type from string
t_type get_enum_type(const char *str)
{
  int i;
  for (i = 0; i < type_size; i++)
    if (strcmp(str, type_str[i]) == 0)
      return i;
  return -1;
}

const char *symbol_table_filename = "ts.txt";

void create_list(t_list *p)
{
  *p = NULL;
}

int insert_order(t_list *p, char *name, t_type datatype, char *value, int length)
{
  int result = -1;
  t_node *new = (t_node *)malloc(sizeof(t_node));

  if (!new)
    return NO_MEMORY;

  while (*p && ((result = (strcmp((*p)->name, name))) < 0))
    p = &(*p)->next;

  if (result == 0)
    return DUPLICATED;
  strcpy(new->name, name);
  new->datatype = datatype;
  strcpy(new->value, value);

  new->length = length;

  new->next = *p;

  *p = new;

  return SUCCESS;
}

int insert_integer(t_list *p, int lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(lexeme, "%d", lex);
  sprintf(name, "_%d", lex);

  result = insert_order(p, name, constant_int, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_real(t_list *p, double lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(name, "_%lf", lex);
  sprintf(lexeme, "%lf", lex);

  result = insert_order(p, name, constant_real, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_string(t_list *p, char *lex)
{
  int result = -1;
  char name[100];

  sprintf(name, "_%s", lex);

  result = insert_order(p, lex, constant_str, name, strlen(lex));

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_variable(t_list *p, char *lex, t_type datatype)
{
  int result = -1;

  result = insert_order(p, lex, datatype, " ", 0);
  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

void save_table_in_file(t_list *p)
{
  FILE *symbol_table_file = fopen(symbol_table_filename, "w+");
  if (symbol_table_file == NULL)
  {
    fputs("File error", stderr);
    exit(1);
  }

  fprintf(symbol_table_file, " %-25s | %-16s | %-30s | %-10s\n",
          "NOMBRE", "TIPODATO", "VALOR", "LONGITUD");

  for (int i = 0; i < 90; ++i)
  {
    fprintf(symbol_table_file, "-");
  }
  fprintf(symbol_table_file, "\n");

  while (*p)
  {
    char length[10];
    if ((*p)->length == 0)
    {
      strcpy(length, "-");
    }
    else
    {
      sprintf(length, "%d", (*p)->length);
    }

    fprintf(symbol_table_file, " %-25s | %-16s | %-30s | %-10s\n",
            (*p)->name, type_str[(*p)->datatype], (*p)->value, length);
    p = &(*p)->next;
  }

  fclose(symbol_table_file);

  printf("\nFile closed and symbol table saved without errors.\n");
}