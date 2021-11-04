%{
  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ts.h"
  #include "queue.h"
  #include "stack.h"
  #include "rpn.h"

  #define COLOR_RED "\033[1;31m"
  #define COLOR_RESET "\033[0m"

  int lineno;

  int yystopparser = 0;

  FILE *yyin;

  char *yyltext;
  char *yytext;

  // Rules
  char* rule[55] = {
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
    "R45. COMPARATION -> ITEM COMPARATOR ITEM",
    "R46. COMPARATOR -> ==",
    "R47. COMPARATOR -> <",
    "R48. COMPARATOR -> <=",
    "R49. COMPARATOR -> >",
    "R50. COMPARATOR -> >=",
    "R51. COMPARATOR -> !=",
    "R52. ITERATION -> while ( CONDITION ) CODE endwhile",
    "R53. ITERATION -> while id in LIST do CODE endwhile",
  };

  // Symbol Table
  list_t symbol_table;

  // Identifier
  char identifier[30];

  // Reverse Polish Notation
  rpn_t *rpn;

  unsigned int item_quantity;

  // Queues
  queue_t* variable_queue;
  queue_t* datatype_queue;
  queue_t* branch_queue;

  // Stack
  stack_t* cell_stack;

  int yylex();
  int yyerror(char *);

  void show_help(char *);

  char* delete_quotes(char *);
%}

%union {
  int int_val;
  double real_val;
  char *str_val;
}

// Tokens
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

// Syntax & Grammar
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
  type_t datatype;

  // load variable type
  while(!queue_is_empty(variable_queue)) {
    // dequeue variable
    variable_elem = dequeue(variable_queue);
    strcpy(identifier, variable_elem);

    // dequeue datatype as enum type
    str_datatype = dequeue(datatype_queue);
    datatype = get_enum_type(str_datatype);

    // insert variable to symbol table
    insert_variable(&symbol_table, identifier, datatype);
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
  enqueue(datatype_queue, get_string_type(integer));

  puts(rule[15]);
} | real_type {
  // enqueue real type
  enqueue(datatype_queue, get_string_type(real));

  puts(rule[16]);
} | string_type {
  // enqueue string type
  enqueue(datatype_queue, get_string_type(str));

  puts(rule[17]);
};


ASSIGNMENT: id op_assign ASSIGNMENT {
  strcpy(identifier, strdup($1));

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(identifier));
  add_lexeme_to_rpn(rpn, (lexeme_t*)":=");

  puts(rule[18]);
} | id op_assign EXPRESSION {
  strcpy(identifier, strdup($1));

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(identifier));
  add_lexeme_to_rpn(rpn, (lexeme_t*)":=");

  puts(rule[19]);
};

EXPRESSION: EXPRESSION op_sum TERM {
  add_lexeme_to_rpn(rpn, (lexeme_t*)"+");

  puts(rule[20]);
} | EXPRESSION op_sub TERM {
  add_lexeme_to_rpn(rpn, (lexeme_t*)"-");

  puts(rule[21]);
} | TERM {
  puts(rule[22]);
};

TERM: TERM op_mult FACTOR {
  add_lexeme_to_rpn(rpn, (lexeme_t*)"*");

  puts(rule[23]);
} | TERM op_div FACTOR {
  add_lexeme_to_rpn(rpn, (lexeme_t*)"/");

  puts(rule[24]);
} | FACTOR {
  puts(rule[25]);
};

FACTOR: open_parenthesis EXPRESSION close_parenthesis {
  puts(rule[26]);
} | id {
  strcpy(identifier, strdup($1));
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(identifier));

  puts(rule[27]);
} | CONSTANT {
  puts(rule[28]);
} | LENGTH {
  puts(rule[29]);
};

CONSTANT: int_constant {
  int integer = $1;
  insert_integer(&symbol_table, integer);

  char integer_lexeme[100];

  sprintf(integer_lexeme, "%d", integer);

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(integer_lexeme));

  puts(rule[30]);
} | real_constant {
  double real = $1;
  insert_real(&symbol_table, real);

  char real_lexeme[100];
  sprintf(real_lexeme, "%f", real);

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(real_lexeme));

  puts(rule[31]);
} | string_constant {
  char* string = delete_quotes($1);
  insert_string(&symbol_table, strdup(string));
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(string));

  puts(rule[32]);
};


LENGTH: fun_long open_parenthesis LIST close_parenthesis {
  char item_quantity_str[100];
  sprintf(item_quantity_str, "%d", item_quantity);

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(item_quantity_str));

  puts(rule[33]);
};

LIST: open_bracket ITEMS close_bracket {
  puts(rule[34]);
};

ITEMS: ITEMS comma ITEM {
  ++item_quantity;

  puts(rule[35]);
} | ITEM {
  item_quantity = 1;

  puts(rule[36]);
};

ITEM: CONSTANT {
  puts(rule[37]);
} | id {
  strcpy(identifier, strdup($1));

  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(identifier));

  puts(rule[38]);
};


INPUT: op_get id {
  puts(rule[39]);
};

OUTPUT: op_display EXPRESSION {
  puts(rule[40]);
};


DECISION: op_if open_parenthesis CONDITION close_parenthesis CODE op_endif {
  // define jump to end of if statement
  int start_cell = pop_from_stack(cell_stack);
  int actual_cell = get_actual_cell_from_rpn(rpn);

  char target_cell[100];

  sprintf(target_cell, "#%d", actual_cell + 1);

  set_lexeme_from_rpn(rpn, start_cell, (lexeme_t*)strdup(target_cell));

  puts(rule[41]);
} | op_if open_parenthesis CONDITION close_parenthesis CODE op_else {
  // add jump to end of if statement
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("BI"));
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("JMP"));

  // define jump to else statement
  int start_cell = pop_from_stack(cell_stack);
  int actual_cell = get_actual_cell_from_rpn(rpn);

  char target_cell[100];

  sprintf(target_cell, "#%d", actual_cell + 1);

  set_lexeme_from_rpn(rpn, start_cell, (lexeme_t*)strdup(target_cell));

  push_to_stack(cell_stack, get_actual_cell_from_rpn(rpn));
} CODE op_endif {
  // define jump to end of if statement
  int start_cell = pop_from_stack(cell_stack);
  int actual_cell = get_actual_cell_from_rpn(rpn);

  char target_cell[100];

  sprintf(target_cell, "#%d", actual_cell + 1);

  set_lexeme_from_rpn(rpn, start_cell, (lexeme_t*)strdup(target_cell));

  puts(rule[42]);
};

CONDITION: COMPARATION op_and COMPARATION {
  yyerror("Error: 'and' operator is not supported");

  puts(rule[43]);
} | COMPARATION op_or COMPARATION {
  yyerror("Error: 'or' operator is not supported");

  puts(rule[44]);
} | COMPARATION {
  // add jump to end of if statement or else
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("JMP"));
  int actual_cell = get_actual_cell_from_rpn(rpn);
  push_to_stack(cell_stack, actual_cell);

  puts(rule[45]);
};

COMPARATION: ITEM COMPARATOR ITEM {
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("CMP"));

  char* branch = dequeue(branch_queue);
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(branch));

  puts(rule[46]);
};

COMPARATOR: op_eq {
  enqueue(branch_queue, "BNE");

  puts(rule[47]);
} | op_lt {
  enqueue(branch_queue, "BGE");

  puts(rule[48]);
} | op_le {
  enqueue(branch_queue, "BGT");

  puts(rule[49]);
} | op_gt {
  enqueue(branch_queue, "BGE");

  puts(rule[50]);
} | op_ge {
  enqueue(branch_queue, "BLT");

  puts(rule[51]);
} | op_ne {
  enqueue(branch_queue, "BEQ");

  puts(rule[52]);
};


ITERATION: op_while {
  // add start etiquete
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("ET"));
  int actual_cell = get_actual_cell_from_rpn(rpn);
  push_to_stack(cell_stack, actual_cell);
} open_parenthesis CONDITION close_parenthesis {
  // add jump to end of while statement
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("JMP"));
  int actual_cell = get_actual_cell_from_rpn(rpn);
  push_to_stack(cell_stack, actual_cell);

  puts(rule[53]);
} CODE op_endwhile {
  // define jump to start of while statement
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup("BI"));

  int start_cell = pop_from_stack(cell_stack);

  char target_cell[100];

  sprintf(target_cell, "#%d", start_cell);
  add_lexeme_to_rpn(rpn, (lexeme_t*)strdup(target_cell));

  int actual_cell = get_actual_cell_from_rpn(rpn);

  sprintf(target_cell, "#%d", actual_cell + 1);
  set_lexeme_from_rpn(rpn, start_cell, (lexeme_t*)strdup(target_cell));

  puts(rule[54]);
} | op_while id op_in LIST op_do CODE op_endwhile {
  yyerror("Error: 'while' operator is not supported");

  puts(rule[54]);
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

  create_list(&symbol_table);
  variable_queue = create_queue();
  datatype_queue = create_queue();
  branch_queue = create_queue();
  cell_stack = create_stack();

  rpn = create_rpn();

  yyparse();

  save_table_in_file(&symbol_table);

  save_lexemes_in_file(rpn);

  free_rpn(rpn);

  fclose(yyin);

  return EXIT_SUCCESS;
}

// Help
void show_help(char* app_name) {
  printf("%s [code_file]\n\n", app_name);
  printf("Example: %s %s\n\n", app_name, "./tests/prueba.txt");
}

// Error
int yyerror(char *error) {
  fprintf(stderr, COLOR_RED "\nline %d: %s\n" COLOR_RESET, lineno, error);
  fclose(yyin);
  exit(1);
}

char *delete_quotes(char *lex)
{
  char *str = lex;
  char *start_str = str;
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
  return start_str;
}
