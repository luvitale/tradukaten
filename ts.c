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
char *getStringType(enum type t)
{
  return (char *)type_str[t];
}

// get enum type from string
enum type getEnumType(const char *str)
{
  int i;
  for (i = 0; i < type_size; i++)
    if (strcmp(str, type_str[i]) == 0)
      return i;
  return -1;
}

const char *symbol_table_filename = "ts.txt";

void createList(t_list *p)
{
  *p = NULL;
}

int insertOrder(t_list *p, char *name, enum type datatype, char *value, int length)
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

int insertInteger(t_list *p, int lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(lexeme, "%d", lex);
  sprintf(name, "_%d", lex);

  result = insertOrder(p, name, constant_int, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insertReal(t_list *p, double lex)
{
  int result = -1;
  char name[100];
  char lexeme[100];

  sprintf(name, "_%lf", lex);
  sprintf(lexeme, "%lf", lex);

  result = insertOrder(p, name, constant_real, lexeme, 0);

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

int insertString(t_list *p, char *lex)
{
  int result = -1;
  char name[100];

  char *newName = deleteQuotes(lex);

  sprintf(name, "_%s", newName);

  result = insertOrder(p, name, constant_str, newName, strlen(newName));

  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

char *deleteQuotes(char *lex)
{
  char *str = lex;
  char *startStr = str;
  while (*lex)
  {
    if (*lex != '"')
    {
      (*str) = (*lex);
      str++;
    }
    lex++;
  }
  *str = '\0';
  return startStr;
}

int insertVariable(t_list *p, char *lex, enum type datatype)
{
  int result = -1;

  result = insertOrder(p, lex, datatype, " ", 0);
  if (result == DUPLICATED)
  {
    return DUPLICATED;
  }

  return SUCCESS;
}

void saveTableInFile(t_list *p)
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