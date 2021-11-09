#include "ts.h"

#define FILENAME "ts.txt"

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
char *get_string_type(type_t t)
{
  return (char *)type_str[t];
}

// get enum type from string
type_t get_enum_type(const char *str)
{
  int i;
  for (i = 0; i < type_size; i++)
    if (strcmp(str, type_str[i]) == 0)
      return i;
  return -1;
}

void create_list(list_t *p)
{
  *p = NULL;
}

int insert_order(list_t *p, char *name, type_t datatype, char *value, int length)
{
  int result = -1;
  node_t *new = (node_t *)malloc(sizeof(node_t));

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

int insert_integer(list_t *p, int lex)
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

int insert_real(list_t *p, double lex)
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

int insert_string(list_t *p, char *lex)
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

int insert_variable(list_t *p, char *lex, type_t datatype)
{
  int result = -1;

  result = insert_order(p, lex, datatype, " ", 0);
  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

// Delete list
void delete_list(list_t *p)
{
  node_t *aux;
  while (*p)
  {
    aux = *p;
    *p = (*p)->next;
    free(aux);
  }
}

void save_table_in_file(list_t *p)
{
  FILE *symbol_table_file = fopen(FILENAME, "w+");

  if (!symbol_table_file)
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