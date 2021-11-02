%{
  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ts.h"
  #include "queue.h"

  #define COLOR_RED "\033[1;31m"
  #define COLOR_RESET "\033[0m"

  int lineno;

  int yystopparser = 0;

  FILE *yyin;

  char *yyltext;
  char *yytext;

  char* rule[56] = {
    "R0. PROGRAM -> CODE",
    "R1. CODE -> CODE BLOCK",
    "R2. CODE -> BLOCK",
    "R3. BLOCK -> DECLARATION;",
    "R4. BLOCK -> ASSIGNMENT;",
    "R5. BLOCK -> INPUT;",
    "R6. BLOCK -> OUTPUT;",
    "R7. BLOCK -> LENGTH;",
    "R8. BLOCK -> DECISION",
    "R9. BLOCK -> ITERATION",
    "R10. DECLARATION -> dim [VARIABLES] as [DATATYPES]",
    "R11. VARIABLES -> VARIABLES, id",
    "R12. VARIABLES -> id",
    "R13. DATATYPES -> DATATYPES, DATATYPE",
    "R14. DATATYPES -> DATATYPE",
    "R15. DATATYPE -> int_type",
    "R16. DATATYPE -> real_type",
    "R17. DATATYPE -> string_type",
    "R18. ASSIGNMENT -> id := ASSIGNMENT",
    "R19. ASSIGNMENT -> id := EXPRESSION",
    "R20. EXPRESSION -> EXPRESSION + TERM",
    "R21. EXPRESSION -> EXPRESSION - TERM",
    "R22. EXPRESSION -> TERM",
    "R23. TERM -> TERM * FACTOR",
    "R24. TERM -> TERM / FACTOR",
    "R25. TERM -> FACTOR",
    "R26. FACTOR -> ( EXPRESSION )",
    "R27. FACTOR -> id",
    "R28. FACTOR -> CONSTANT",
    "R29. FACTOR -> LENGTH",
    "R30. CONSTANT -> int_constant",
    "R31. CONSTANT -> real_constant",
    "R32. CONSTANT -> string_constant",
    "R33. LENGTH -> long ( LIST )",
    "R34. LIST -> [ ITEMS ]",
    "R35. ITEMS -> ITEMS, ITEM",
    "R36. ITEMS -> ITEM",
    "R37. ITEM -> CONSTANT",
    "R38. ITEM -> id",
    "R39. INPUT -> get id",
    "R40. OUTPUT -> display EXPRESSION",
    "R41. DECISION -> if ( CONDITION ) CODE endif",
    "R41. DECISION -> if ( CONDITION ) CODE else CODE endif",
    "R42. CONDITION -> CONDITION && COMPARATION",
    "R43. CONDITION -> CONDITION || COMPARATION",
    "R44. CONDITION -> COMPARATION",
    "R45. COMPARATION -> EXPRESSION COMPARATOR EXPRESSION",
    "R46. COMPARATION -> ( CONDITION )",
    "R47. COMPARATOR -> ==",
    "R48. COMPARATOR -> <",
    "R49. COMPARATOR -> <=",
    "R50. COMPARATOR -> >",
    "R51. COMPARATOR -> >=",
    "R52. COMPARATOR -> !=",
    "R53. ITERATION -> while ( CONDITION ) CODE endwhile",
    "R54. ITERATION -> while id in LIST do CODE endwhile"
  };

  char identifier[30];

  t_list symbol_table;

  struct Queue* variable_queue;
  struct Queue* datatype_queue;

  int yylex();
  int yyerror(char *);

  void show_help(char *);
%}

%union {
  int int_val;
  double real_val;
  char *str_val;
}

%token <str_val> id
%token <int_val> int_constant
%token <real_val> real_constant
%token <str_val> string_constant

%token op_assign
%token op_sum op_sub op_mult op_div
%token separator

%token op_eq op_lt op_le op_gt op_ge op_ne
%token op_and op_or

%token op_if op_else op_endif
%token op_while op_endwhile
%token open_parenthesis close_parenthesis

%token op_dim op_as
%token open_bracket close_bracket
%token comma
%token int_type real_type string_type

%token op_display op_get

%token fun_long
%token op_in op_do

%%
PROGRAM: CODE;

CODE: CODE BLOCK {
  puts(rule[1]);
} | BLOCK {
  puts(rule[2]);
};

BLOCK: DECLARATION separator {
  puts(rule[3]);
} | ASSIGNMENT separator {
  puts(rule[4]);
} | LENGTH separator {
  puts(rule[5]);
} | INPUT separator {
  puts(rule[6]);
} | OUTPUT separator {
  puts(rule[7]);
} | DECISION {
  puts(rule[8]);
} | ITERATION {
  puts(rule[9]);
};


DECLARATION: op_dim open_bracket VARIABLES close_bracket op_as open_bracket DATATYPES close_bracket {
  char* variable_elem, *str_datatype;
  enum type datatype;

  while(!isEmpty(variable_queue)) {
    // dequeue variable
    variable_elem = dequeue(variable_queue);
    strcpy(identifier, variable_elem);

    // dequeue datatype as enum type
    str_datatype = dequeue(datatype_queue);
    datatype = getEnumType(str_datatype);

    // insert variable to symbol table
    insertVariable(&symbol_table, identifier, datatype);
  }
  
  puts(rule[10]);
};

VARIABLES: VARIABLES comma id {
  strcpy(identifier, strdup($3));

  // enqueue identifier
  enqueue(variable_queue, strdup(identifier));

  puts(rule[11]);
} | id {
  strcpy(identifier, strdup($1));

  // enqueue identifier
  enqueue(variable_queue, strdup(identifier));

  puts(rule[12]);
};

DATATYPES: DATATYPES comma DATATYPE {
  puts(rule[13]);
} | DATATYPE {
  puts(rule[14]);
};

DATATYPE: int_type {
  // enqueue integer type
  enqueue(datatype_queue, getStringType(integer));

  puts(rule[15]);
} | real_type {
  // enqueue real type
  enqueue(datatype_queue, getStringType(real));

  puts(rule[16]);
} | string_type {
  // enqueue string type
  enqueue(datatype_queue, getStringType(str));

  puts(rule[17]);
};


ASSIGNMENT: id op_assign ASSIGNMENT {
  puts(rule[18]);
} | id op_assign EXPRESSION {
  puts(rule[19]);
};

EXPRESSION: EXPRESSION op_sum TERM {
  puts(rule[20]);
} | EXPRESSION op_sub TERM {
  puts(rule[21]);
} | TERM {
  puts(rule[22]);
};

TERM: TERM op_mult FACTOR {
  puts(rule[23]);
} | TERM op_div FACTOR {
  puts(rule[24]);
} | FACTOR {
  puts(rule[25]);
};

FACTOR: open_parenthesis EXPRESSION close_parenthesis {
  puts(rule[26]);
} | id {
  puts(rule[27]);
} | CONSTANT {
  puts(rule[28]);
} | LENGTH {
  puts(rule[29]);
};

CONSTANT: int_constant {
  int integer = $1;
  insertInteger(&symbol_table, integer);

  puts(rule[30]);
} | real_constant {
  double real = $1;
  insertReal(&symbol_table, real);

  puts(rule[31]);
} | string_constant {
  char* string = $1;
  insertString(&symbol_table, string);

  puts(rule[32]);
};


LENGTH: fun_long open_parenthesis LIST close_parenthesis {
  puts(rule[33]);
};

LIST: open_bracket ITEMS close_bracket {
  puts(rule[34]);
};

ITEMS: ITEMS comma ITEM {
  puts(rule[35]);
} | ITEM {
  puts(rule[36]);
};

ITEM: CONSTANT {
  puts(rule[37]);
} | id {
  puts(rule[38]);
};


INPUT: op_get id {
  puts(rule[39]);
};

OUTPUT: op_display EXPRESSION {
  puts(rule[40]);
};


DECISION: op_if open_parenthesis CONDITION close_parenthesis CODE op_endif {
  puts(rule[41]);
} | op_if open_parenthesis CONDITION close_parenthesis CODE op_else CODE op_endif {
  puts(rule[42]);
};

CONDITION: CONDITION op_and COMPARATION {
  puts(rule[43]);
} | CONDITION op_or COMPARATION {
  puts(rule[44]);
} | COMPARATION {
  puts(rule[45]);
};

COMPARATION: EXPRESSION COMPARATOR EXPRESSION {
  puts(rule[46]);
} | open_parenthesis CONDITION close_parenthesis {
  puts(rule[47]);
};

COMPARATOR: op_eq {
  puts(rule[48]);
} | op_lt {
  puts(rule[49]);
} | op_le {
  puts(rule[50]);
} | op_gt {
  puts(rule[51]);
} | op_ge {
  puts(rule[52]);
} | op_ne {
  puts(rule[53]);
};


ITERATION: op_while open_parenthesis CONDITION close_parenthesis CODE op_endwhile {
  puts(rule[54]);
} | op_while id op_in LIST op_do CODE op_endwhile {
  puts(rule[55]);
};
%%

int main(int argc,char *argv[]) {
  const char* filename = argv[1];

  if( filename && (
      strcmp(filename, "-h") == 0 || strcmp(filename, "--help") == 0
    )
  ) {
    show_help(argv[0]);

    return EXIT_SUCCESS;
  }

  FILE* arg_file = fopen(filename, "rt");

  if (arg_file == NULL) {
    show_help(argv[0]);

    printf("File cannot be opened: %s\n", filename);
    printf("Using standard input\n\n");
  }
  else {
    yyin = arg_file;
  }

  createList(&symbol_table);
  variable_queue = createQueue(500);
  datatype_queue = createQueue(500);

  yyparse();

  saveTableInFile(&symbol_table);

  fclose(yyin);

  return EXIT_SUCCESS;
}

void show_help(char* app_name) {
  printf("%s [code_file]\n\n", app_name);
  printf("Example: %s %s\n\n", app_name, "./tests/prueba.txt");
}

int yyerror(char *error) {
  fprintf(stderr, COLOR_RED "\nline %d: %s\n" COLOR_RESET, lineno, error);
  fclose(yyin);
  exit(1);
}