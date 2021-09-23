#include "ts.h"

int yyerror(char *);

const int CANT_CHARS = 240;
const int CANT_MAX_SYMBOLS = 500;

const char *const type_str[] = {
    [not_type] = "",
    [integer] = "Integer",
    [constant_int] = "Constant Integer",
    [real] = "Real",
    [constant_real] = "Constant Real",
    [string] = "String",
    [constant_str] = "Constant String",
    [identifier] = "ID",
    [constant] = "Constant",
};

void saveFileTS(void)
{
  FILE *symbol_table_file;
  int symbol_num, character;
  symbol_table_file = fopen("ts.txt", "w+");
  if (symbol_table_file == NULL)
  {
    fputs("File error", stderr);
    exit(1);
  }

  fprintf(symbol_table_file, "NOMBRE %93s | TIPODATO %11s | VALOR %94s | LONGITUD %2s\n", " ", " ", " ", " ");
  for (character = 0; character < CANT_CHARS; character++)
    fprintf(symbol_table_file, "-");
  fprintf(symbol_table_file, "\n");

  for (symbol_num = 0; symbol_num < CANT_MAX_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].datatype != not_type)
      fprintf(
          symbol_table_file, "%-100s | %-20s | %-100s | %-11s\n",
          symbol_table[symbol_num].lexeme,
          type_str[symbol_table[symbol_num].datatype],
          symbol_table[symbol_num].value,
          symbol_table[symbol_num].length);
    else
      break;
  }
  fclose(symbol_table_file);

  printf("\nFile closed and symbol table saved without errors.\n");
}

int loadToTS(char *lexeme, enum type symbol_type)
{
  int symbol_num;
  int isRepeated = 0;
  char lexeme_with_underscore[strlen(lexeme) + 1];

  for (symbol_num = 0; symbol_num < CANT_MAX_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].isStored == 1)
    {
      if (strcmp(lexeme, symbol_table[symbol_num].lexeme) == 0)
      {
        if (symbol_table[symbol_num].value == 0)
        {
          yyerror("variable duplicated");
        }

        return symbol_num;
      }
      else
      {
        strcpy(lexeme_with_underscore, "_");
        strcat(lexeme_with_underscore, lexeme);
        if (strcmp(lexeme_with_underscore, symbol_table[symbol_num].lexeme) == 0)
        {
          return symbol_num;
        }
      }
    }
  }

  for (symbol_num = 0; symbol_num < CANT_MAX_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].isStored == 0)
    {

      if (strstr(type_str[symbol_type], type_str[constant]))
      {
        strcpy(lexeme_with_underscore, "_");
        strcat(lexeme_with_underscore, lexeme);
        strcpy(symbol_table[symbol_num].lexeme, lexeme_with_underscore);
        strcpy(symbol_table[symbol_num].value, lexeme);
        if (symbol_type == constant_str)
        {
          if (strlen(lexeme) == 0)
          {
            strcpy(symbol_table[symbol_num].length, "-");
          }
          else
          {
            sprintf(symbol_table[symbol_num].length, "%ld", strlen(lexeme) - 2);
          }
        }
      }
      else
      {
        strcpy(symbol_table[symbol_num].lexeme, lexeme);
      }

      symbol_table[symbol_num].datatype = symbol_type;
      symbol_table[symbol_num].isStored = 1;

      return symbol_num;
    }
  }

  return symbol_num;
}

void assignConstantValue()
{
  int symbol_num;
  for (symbol_num = 0; symbol_num < CANT_MAX_SYMBOLS - 1; ++symbol_num)
  {
    if (symbol_table[symbol_num].isStored == 1)
    {
      if (symbol_table[symbol_num].datatype == constant)
      {
        char *aux = symbol_table[symbol_num - 1].lexeme;
        strcpy(symbol_table[symbol_num].value, ++aux);
      }
    }
  }
}
