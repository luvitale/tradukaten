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

void create_list(table_t *p)
{
  *p = NULL;
}

type_t get_lex_type(table_t *p, char *name)
{
  table_t l = *p;
  while (l != NULL)
  {
    if (strcmp(l->name, name) == 0)
      return l->datatype;
    l = l->next;
  }
  return -1;
}

int insert_order(table_t *p, char *name, type_t datatype, char *value, int length)
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

char *replace_char(char *str, char find, char replace)
{
  char *current_pos = strchr(str, find);
  while (current_pos)
  {
    *current_pos = replace;
    current_pos = strchr(current_pos, find);
  }
  return str;
}

char *get_string_name(char *str)
{
  char *name = (char *)malloc(sizeof(char) * 100);
  sprintf(name, "%s", str);
  replace_char(name, ' ', '_');
  replace_char(name, '.', '_');
  replace_char(name, ':', '@');
  return name;
}

int insert_integer(table_t *p, int lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(name, "_i_%d", lex);
  sprintf(lexeme, "%d", lex);

  result = insert_order(p, name, constant_int, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_real(table_t *p, double lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(name, "_r_%lf", lex);
  sprintf(lexeme, "%lf", lex);

  sprintf(name, "%s", replace_char(name, '.', '_'));

  result = insert_order(p, name, constant_real, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_string(table_t *p, char *lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(name, "_s_%s", get_string_name(lex));
  sprintf(lexeme, "%s", lex);

  result = insert_order(p, name, constant_str, lexeme, strlen(lex));

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insert_variable(table_t *p, char *lex, type_t datatype)
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
void delete_list(table_t *p)
{
  node_t *aux;
  while (*p)
  {
    aux = *p;
    *p = (*p)->next;
    free(aux);
  }
}

void save_table_in_file(table_t *p)
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