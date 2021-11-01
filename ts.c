#include "ts.h"

int checkIfItIsDuplicated(char *lexeme);
void loadConstantToSymbolTable(int symbol_num, char *lexeme, enum type symbol_type);
void loadIdToSymbolTable(int symbol_num, char *lexeme);
int loadToSymbolTable(char *lexeme, enum type symbol_type);

int yyerror(char *);

const int CHARS_QUANTITY = 110;
const int MAX_NUM_SYMBOLS = 500;
const int NOT_DUPLICATED = -1;

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

  // Header
  fprintf(symbol_table_file, "%-35s | %-20s | %-35s | %-11s\n",
          "NOMBRE",
          "TIPODATO",
          "VALOR",
          "LONGITUD");
  for (character = 0; character < CHARS_QUANTITY; character++)
    fprintf(symbol_table_file, "-");
  fprintf(symbol_table_file, "\n");

  // ID & Constants
  for (symbol_num = 0; symbol_num < MAX_NUM_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].datatype != not_type)
      fprintf(
          symbol_table_file, "%-35s | %-20s | %-35s | %-11s\n",
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

int checkIfItIsDuplicated(char *lexeme)
{
  int symbol_num;
  char lexeme_with_underscore[strlen(lexeme) + 1];
  for (symbol_num = 0; symbol_num < MAX_NUM_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].isStored == 0)
      return NOT_DUPLICATED;

    // If it is lexeme (ID)
    if (strcmp(lexeme, symbol_table[symbol_num].lexeme) == 0)
    {
      if (symbol_table[symbol_num].value == 0)
      {
        yyerror("variable duplicated");
      }

      return symbol_num;
    }

    // Else
    else
    {
      // Check Lexeme with underscore (Constant)
      sprintf(lexeme_with_underscore, "_%s", lexeme);
      if (strcmp(lexeme_with_underscore, symbol_table[symbol_num].lexeme) == 0)
      {
        return symbol_num;
      }
    }
  }

  return symbol_num;
}

void loadConstantToSymbolTable(int symbol_num, char *lexeme, enum type symbol_type)
{
  char lexeme_with_underscore[strlen(lexeme) + 1];

  sprintf(lexeme_with_underscore, "_%s", lexeme);

  strcpy(symbol_table[symbol_num].lexeme, lexeme_with_underscore);
  strcpy(symbol_table[symbol_num].value, lexeme);

  // If it is String
  if (symbol_type == constant_str)
  {
    // String Length = 0
    if (strlen(lexeme) == 0)
    {
      strcpy(symbol_table[symbol_num].length, "-");
    }
    // String Length != 0
    else
    {
      sprintf(symbol_table[symbol_num].length, "%ld", strlen(lexeme) - 2);
    }
  }
}

void loadIdToSymbolTable(int symbol_num, char *lexeme)
{
  strcpy(symbol_table[symbol_num].lexeme, lexeme);
}

int loadToSymbolTable(char *lexeme, enum type symbol_type)
{
  int symbol_num;
  char lexeme_with_underscore[strlen(lexeme) + 1];

  for (symbol_num = 0; symbol_num < MAX_NUM_SYMBOLS; ++symbol_num)
  {
    if (symbol_table[symbol_num].isStored == 0)
    {

      if (strstr(type_str[symbol_type], type_str[constant]))
      {
        loadConstantToSymbolTable(symbol_num, lexeme, symbol_type);
      }
      else
      {
        loadIdToSymbolTable(symbol_num, lexeme);
      }

      symbol_table[symbol_num].datatype = symbol_type;
      symbol_table[symbol_num].isStored = 1;

      return symbol_num;
    }
  }

  return symbol_num;
}

int loadToTS(char *lexeme, enum type symbol_type)
{
  int symbol_num;

  int duplicated_symbol_num = checkIfItIsDuplicated(lexeme);

  if (duplicated_symbol_num != NOT_DUPLICATED)
    return duplicated_symbol_num;

  return loadToSymbolTable(lexeme, symbol_type);
}

void assignConstantValue()
{
  int symbol_num;
  for (symbol_num = 0; symbol_num < MAX_NUM_SYMBOLS - 1; ++symbol_num)
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
