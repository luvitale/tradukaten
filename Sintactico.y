%{
  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "ts.h"
  #include "utils/queue.h"
  #include "utils/stack.h"
  #include "utils/int_list.h"
  #include "rpn.h"

  #define COLOR_RED "\033[1;31m"
  #define COLOR_RESET "\033[0m"

  #define FALSE 0
  #define TRUE 1

  int lineno;

  int yystopparser = 0;

  FILE *yyin;

  char *yyltext;
  char *yytext;

  // Rules
  char *rule[57] = {
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
    "R41. DECISION -> IF_EVALUATOR CODE endif",
    "R42. DECISION -> IF_EVALUATOR CODE else CODE endif",
    "R43. IF_EVALUATOR -> if ( CONDITION )",
    "R44. CONDITION -> CONDITION && comparator",
    "R45. CONDITION -> CONDITION || comparator",
    "R46. CONDITION -> comparator",
    "R47. comparator -> ITEM COMPARATOR ITEM",
    "R48. COMPARATOR -> ==",
    "R49. COMPARATOR -> <",
    "R50. COMPARATOR -> <=",
    "R51. COMPARATOR -> >",
    "R52. COMPARATOR -> >=",
    "R53. COMPARATOR -> !=",
    "R54. ITERATION -> WHILE_EVALUATOR ( CONDITION ) CODE endwhile",
    "R55. ITERATION -> WHILE_EVALUATOR id in LIST do CODE endwhile",
    "R56. WHILE_EVALUATOR -> while ( CONDITION )",
  };

  // Symbol Table
  table_t symbol_table;

  // Actual Item
  char actual_item[30];
  char identifier[30];

  // ET
  int_list_t et_list;

  // Reverse Polish Notation
  rpn_t *rpn;

  // Counters
  unsigned int item_quantity;

  // Util variables
  int is_while_loop = FALSE;
  char logical_operator[3];
  char comparator[3];
  char comparator1[3];
  char comparator2[3];
  char branch[3];
  int first_condition_jump_cell;
  int show = TRUE;

  // Queues
  queue_str_t* variable_queue;
  queue_str_t* datatype_queue;
  queue_str_t* branch_queue;
  queue_str_t* while_queue;

  // Stack
  stack_str_t* cell_stack;
  stack_str_t* is_and_stack;

  int yylex();
  int yyerror(char *);

  void show_help(char *);

  char *delete_quotes(char *);

  char *get_corresponding_branch(char *);
  char *get_opposite_branch(char *);
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
PROGRAM: CODE {
  save_table_in_file(&symbol_table);

  save_cells_in_file(rpn);

  rpn_assemble(rpn, &symbol_table, &et_list);
};

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

  add_cell_to_rpn(rpn, (cell_t*)strdup(identifier));
  add_cell_to_rpn(rpn, (cell_t*)":=");

  puts(rule[18]);
} | id op_assign EXPRESSION {
  strcpy(identifier, strdup($1));

  add_cell_to_rpn(rpn, (cell_t*)strdup(identifier));
  add_cell_to_rpn(rpn, (cell_t*)":=");

  puts(rule[19]);
};

EXPRESSION: EXPRESSION op_sum TERM {
  add_cell_to_rpn(rpn, (cell_t*)strdup("+"));

  // create result variable
  char result_variable_name[100];
  sprintf(result_variable_name, "@result_%d", get_size_of_rpn(rpn));
  type_t result_variable_datatype = real;
  insert_variable(&symbol_table, result_variable_name, result_variable_datatype);

  puts(rule[20]);
} | EXPRESSION op_sub TERM {
  add_cell_to_rpn(rpn, (cell_t*)strdup("-"));

  // create result variable
  char result_variable_name[100];
  sprintf(result_variable_name, "@result_%d", get_size_of_rpn(rpn));
  type_t result_variable_datatype = real;
  insert_variable(&symbol_table, result_variable_name, result_variable_datatype);

  puts(rule[21]);
} | TERM {
  puts(rule[22]);
};

TERM: TERM op_mult FACTOR {
  add_cell_to_rpn(rpn, (cell_t*)strdup("*"));

  // create result variable
  char result_variable_name[100];
  sprintf(result_variable_name, "@result_%d", get_size_of_rpn(rpn));
  type_t result_variable_datatype = real;
  insert_variable(&symbol_table, result_variable_name, result_variable_datatype);

  puts(rule[23]);
} | TERM op_div FACTOR {
  add_cell_to_rpn(rpn, (cell_t*)strdup("/"));

  // create result variable
  char result_variable_name[100];
  sprintf(result_variable_name, "@result_%d", get_size_of_rpn(rpn));
  type_t result_variable_datatype = real;
  insert_variable(&symbol_table, result_variable_name, result_variable_datatype);

  puts(rule[24]);
} | FACTOR {
  puts(rule[25]);
};

FACTOR: open_parenthesis EXPRESSION close_parenthesis {
  puts(rule[26]);
} | id {
  strcpy(identifier, strdup($1));
  add_cell_to_rpn(rpn, (cell_t*)strdup(identifier));

  puts(rule[27]);
} | CONSTANT {
  puts(rule[28]);
} | LENGTH {
  puts(rule[29]);
};

CONSTANT: int_constant {
  int integer = $1;
  insert_integer(&symbol_table, integer);

  sprintf(actual_item, "_i_%d", integer);

  strcpy(actual_item, get_string_name(actual_item));

  if (show == TRUE) {
    add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item));
  }

  puts(rule[30]);
} | real_constant {
  double real = $1;
  insert_real(&symbol_table, real);

  sprintf(actual_item, "_r_%lf", real);
  strcpy(actual_item, get_string_name(actual_item));

  if (show == TRUE) {
    add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item));
  }

  puts(rule[31]);
} | string_constant {
  char identifier[30];
  strcpy(identifier, delete_quotes($1));
  strcpy(actual_item, strdup(identifier));

  if (strcmp(identifier, "") != 0) {
    insert_string(&symbol_table, strdup(actual_item));
  }

  sprintf(actual_item, "_s_%s", get_string_name(actual_item));

  if (show == TRUE) {
    add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item));
  }

  puts(rule[32]);
};


LENGTH: fun_long open_parenthesis LIST close_parenthesis {
  char item_quantity_str[100];
  
  insert_integer(&symbol_table, item_quantity);
  sprintf(item_quantity_str, "_i_%d", item_quantity);
  strcpy(item_quantity_str, get_string_name(item_quantity_str));

  add_cell_to_rpn(rpn, (cell_t*)strdup(item_quantity_str));

  puts(rule[33]);
};

LIST: open_bracket {
  show = FALSE;
} ITEMS close_bracket {
  show = TRUE;

  puts(rule[34]);
};

ITEMS: ITEMS comma ITEM {
  ++item_quantity;

  if (is_while_loop) {
    enqueue(while_queue, strdup(actual_item));
  }

  puts(rule[35]);
} | ITEM {
  item_quantity = 1;

  if (is_while_loop) {
    enqueue(while_queue, strdup(actual_item));
  }

  puts(rule[36]);
};

ITEM: CONSTANT {
  puts(rule[37]);
} | id {
  strcpy(actual_item, strdup($1));

  if (show == TRUE) {
    add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item));
  }

  puts(rule[38]);
};


INPUT: op_get id {
  add_cell_to_rpn(rpn, (cell_t*)strdup($2));
  add_cell_to_rpn(rpn, (cell_t*)strdup("GET"));

  puts(rule[39]);
};

OUTPUT: op_display EXPRESSION {
  add_cell_to_rpn(rpn, (cell_t*)strdup("DSP"));

  puts(rule[40]);
};


DECISION: IF_EVALUATOR CODE op_endif {
  // define jump to end of if statement
  int condition_jump_cell = pop_from_stack(cell_stack);
  int actual_cell_num = get_size_of_rpn(rpn);

  char target_cell[5];

  sprintf(target_cell, "#%d", actual_cell_num + 1);

  add_element_to_list(&et_list, actual_cell_num + 1);

  set_cell_from_rpn(rpn, condition_jump_cell, (cell_t*)strdup(target_cell));

  // when there is AND operator
  if (pop_from_stack(is_and_stack) == TRUE) {
    condition_jump_cell = pop_from_stack(cell_stack);
    sprintf(target_cell, "#%d", actual_cell_num + 1);

    add_element_to_list(&et_list, actual_cell_num + 1);
    
    set_cell_from_rpn(rpn, condition_jump_cell, (cell_t*)strdup(target_cell));
  }

  puts(rule[41]);
} | IF_EVALUATOR CODE op_else {
  // add jump to end of if statement
  add_cell_to_rpn(rpn, (cell_t*)strdup("BI"));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  // define jump to else statement
  int start_cell = pop_from_stack(cell_stack);
  int actual_cell_num = get_size_of_rpn(rpn);

  char target_cell[5];

  sprintf(target_cell, "#%d", actual_cell_num + 1);
  
  add_element_to_list(&et_list, actual_cell_num + 1);
    
  set_cell_from_rpn(rpn, start_cell, (cell_t*)strdup(target_cell));

  push_to_stack(cell_stack, get_size_of_rpn(rpn));
} CODE op_endif {
  // define jump to end of if statement
  int start_cell = pop_from_stack(cell_stack);
  int actual_cell_num = get_size_of_rpn(rpn);

  char target_cell[5];

  sprintf(target_cell, "#%d", actual_cell_num + 1);

  add_element_to_list(&et_list, actual_cell_num + 1);
    
  set_cell_from_rpn(rpn, start_cell, (cell_t*)strdup(target_cell));

  puts(rule[42]);
};

IF_EVALUATOR: op_if {
  add_cell_to_rpn(rpn, (cell_t*)strdup("ET"));
} open_parenthesis CONDITION close_parenthesis {
  // Single condition
  if (strcmp(logical_operator, "\0") == 0) {
    char branch[3];
    strcpy(branch, strdup(get_opposite_branch(comparator)));
    
    add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
    add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
    add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

    push_to_stack(cell_stack, get_size_of_rpn(rpn));

    push_to_stack(is_and_stack, FALSE);
  }

  strcpy(logical_operator, "\0");

  puts(rule[43]);
};

CONDITION: COMPARATION op_and {
  strcpy(comparator1, comparator);
  strcpy(logical_operator, "&&");
  
  strcpy(branch, strdup(get_opposite_branch(comparator1)));

  add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  push_to_stack(cell_stack, get_size_of_rpn(rpn));
} COMPARATION {
  strcpy(comparator2, comparator);

  strcpy(branch, strdup(get_opposite_branch(comparator2)));

  add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  push_to_stack(cell_stack, get_size_of_rpn(rpn));
  
  push_to_stack(is_and_stack, TRUE);

  puts(rule[44]);
} | COMPARATION op_or {
  strcpy(comparator1, comparator);
  
  strcpy(logical_operator, "||");

  // Comparator 1
  strcpy(branch, strdup(get_corresponding_branch(comparator1)));

  add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  first_condition_jump_cell = get_size_of_rpn(rpn);
} COMPARATION {
  strcpy(comparator2, comparator);

  // Comparator 2
  strcpy(branch, strdup(get_opposite_branch(comparator2)));

  add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  push_to_stack(cell_stack, get_size_of_rpn(rpn));

  // Change jump of first condition
  char target_cell[5];
  sprintf(target_cell, "#%d", get_size_of_rpn(rpn) + 1);
  
  add_element_to_list(&et_list, get_size_of_rpn(rpn) + 1);
    
  set_cell_from_rpn(rpn, first_condition_jump_cell, (cell_t*)strdup(target_cell));

  push_to_stack(is_and_stack, FALSE);

  puts(rule[45]);
} | COMPARATION {
  puts(rule[46]);
};

COMPARATION: ITEM COMPARATOR ITEM {
  puts(rule[47]);
};

COMPARATOR: op_eq {
  strcpy(comparator, strdup("=="));

  puts(rule[48]);
} | op_lt {
  strcpy(comparator, strdup("<"));

  puts(rule[49]);
} | op_le {
  strcpy(comparator, strdup("<="));

  puts(rule[50]);
} | op_gt {
  strcpy(comparator, strdup(">"));

  puts(rule[51]);
} | op_ge {
  strcpy(comparator, strdup(">="));

  puts(rule[52]);
} | op_ne {
  strcpy(comparator, strdup("!="));

  puts(rule[53]);
};

ITERATION: WHILE_EVALUATOR {
  // Single condition
  if (strcmp(logical_operator, "\0") == 0) {
    strcpy(branch, strdup(get_opposite_branch(comparator)));
    
    add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
    add_cell_to_rpn(rpn, (cell_t*)strdup(branch));
    add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

    push_to_stack(cell_stack, get_size_of_rpn(rpn));

    push_to_stack(is_and_stack, FALSE);
  }
} CODE op_endwhile {
  // define jump to start of while statement
  char target_cell[5];

  add_cell_to_rpn(rpn, (cell_t*)strdup("BI"));
  add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

  int actual_cell_num = get_size_of_rpn(rpn);

  // change jump to end of while statement
  int condition_jump_cell = pop_from_stack(cell_stack);
  sprintf(target_cell, "#%d", actual_cell_num + 1);
  
  add_element_to_list(&et_list, actual_cell_num + 1);
    
  set_cell_from_rpn(rpn, condition_jump_cell, (cell_t*)strdup(target_cell));

  // when there is AND operator
  if (pop_from_stack(is_and_stack) == TRUE) {
    condition_jump_cell = pop_from_stack(cell_stack);
    sprintf(target_cell, "#%d", actual_cell_num + 1);
    
    add_element_to_list(&et_list, actual_cell_num + 1);
    
    set_cell_from_rpn(rpn, condition_jump_cell, (cell_t*)strdup(target_cell));
  }

  // change jump to start of while statement
  int start_jump_cell = pop_from_stack(cell_stack);
  sprintf(target_cell, "#%d", start_jump_cell);
  
  add_element_to_list(&et_list, start_jump_cell);

  set_cell_from_rpn(rpn, actual_cell_num, (cell_t*)strdup(target_cell));

  puts(rule[54]);
} | op_while id op_in {
  is_while_loop = TRUE;
  printf("is_while_loop: %d\n", is_while_loop);
} LIST op_do {
  is_while_loop = FALSE;

  int actual_item_num = 1;
  char actual_item_num_str[100];
  char* identifier = strdup($2);
  
  insert_integer(&symbol_table, actual_item_num);
  sprintf(actual_item_num_str, "_i_%d", actual_item_num);
  strcpy(actual_item_num_str, get_string_name(actual_item_num_str));
  add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item_num_str));

  // insert special while @act variable
  insert_variable(&symbol_table, "@act", integer);

  add_cell_to_rpn(rpn, (cell_t*)strdup("@act"));

  add_cell_to_rpn(rpn, (cell_t*)strdup(":="));

  add_cell_to_rpn(rpn, (cell_t*)strdup("ET"));

  push_to_stack(cell_stack, get_size_of_rpn(rpn));

  while (!queue_is_empty(while_queue)) {
    char item[30];
    strcpy(item, strdup(dequeue(while_queue)));
    add_cell_to_rpn(rpn, (cell_t*)strdup(item));
    add_cell_to_rpn(rpn, (cell_t*)strdup(identifier));
    add_cell_to_rpn(rpn, (cell_t*)strdup(":="));

    if (actual_item_num < item_quantity) {
      add_cell_to_rpn(rpn, (cell_t*)strdup("@act"));

      insert_integer(&symbol_table, actual_item_num);
      sprintf(actual_item_num_str, "_i_%d", actual_item_num);
      strcpy(actual_item_num_str, get_string_name(actual_item_num_str));

      add_cell_to_rpn(rpn, (cell_t*)strdup(actual_item_num_str));

      add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
      add_cell_to_rpn(rpn, (cell_t*)strdup("BEQ"));
      add_cell_to_rpn(rpn, (cell_t*)strdup("JMP"));

      push_to_stack(cell_stack, get_size_of_rpn(rpn));
    }
    
    actual_item_num++;
  }

  push_to_stack(cell_stack, get_size_of_rpn(rpn));
} CODE op_endwhile {
  add_cell_to_rpn(rpn, (cell_t*)strdup("@act"));

  int INCREMENT = 1;
  char increment_str[100];
  
  insert_integer(&symbol_table, INCREMENT);
  sprintf(increment_str, "_i_%d", INCREMENT);
  strcpy(increment_str, get_string_name(increment_str));
  add_cell_to_rpn(rpn, (cell_t*)strdup(increment_str));

  add_cell_to_rpn(rpn, (cell_t*)strdup("+"));

  // create result variable
  char result_variable_name[100];
  sprintf(result_variable_name, "@result_%d", get_size_of_rpn(rpn));
  type_t result_variable_datatype = real;
  insert_variable(&symbol_table, result_variable_name, result_variable_datatype);

  add_cell_to_rpn(rpn, (cell_t*)strdup("@act"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(":="));

  char item_quantity_str[100];

  insert_integer(&symbol_table, item_quantity);
  sprintf(item_quantity_str, "_i_%d", item_quantity);
  strcpy(item_quantity_str, get_string_name(item_quantity_str));

  add_cell_to_rpn(rpn, (cell_t*)strdup("@act"));
  add_cell_to_rpn(rpn, (cell_t*)strdup(item_quantity_str));
  add_cell_to_rpn(rpn, (cell_t*)strdup("CMP"));
  add_cell_to_rpn(rpn, (cell_t*)strdup("BLE"));

  int code_cell = pop_from_stack(cell_stack);

  char target_cell[100];

  while(!stack_is_empty(cell_stack)) {
    int cell = pop_from_stack(cell_stack);

    if (stack_is_empty(cell_stack)) {
      sprintf(target_cell, "#%d", cell + 1);
      
    add_element_to_list(&et_list, cell + 1);
    
      add_cell_to_rpn(rpn, (cell_t*)strdup(target_cell));
    }
    else {
      sprintf(target_cell, "#%d", code_cell + 1);

      add_element_to_list(&et_list, code_cell + 1);
    
      set_cell_from_rpn(rpn, cell, (cell_t*)strdup(target_cell));
    }
  }

  puts(rule[55]);
};

WHILE_EVALUATOR: op_while open_parenthesis {
  // add start etiquete
  add_cell_to_rpn(rpn, (cell_t*)strdup("ET"));
  int actual_cell_num = get_size_of_rpn(rpn);
  push_to_stack(cell_stack, actual_cell_num);
} CONDITION close_parenthesis {
  puts(rule[56]);
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
  while_queue = create_queue();
  cell_stack = create_stack();
  is_and_stack = create_stack();
  create_int_list(&et_list);

  rpn = create_rpn();

  yyparse();

  delete_list(&symbol_table);
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

char *get_corresponding_branch(char *comparator) {
  if (strcmp(comparator, "==") == 0) {
    return "BEQ";
  }
  else if (strcmp(comparator, "!=") == 0) {
    return "BNE";
  }
  else if (strcmp(comparator, "<") == 0) {
    return "BLT";
  }
  else if (strcmp(comparator, ">") == 0) {
    return "BGT";
  }
  else if (strcmp(comparator, "<=") == 0) {
    return "BLE";
  }
  else if (strcmp(comparator, ">=") == 0) {
    return "BGE";
  }
  else {
    return "\0";
  }
}

char *get_opposite_branch(char *comparator) {
  if (strcmp(comparator, "==") == 0) {
    return "BNE";
  }
  else if (strcmp(comparator, "!=") == 0) {
    return "BEQ";
  }
  else if (strcmp(comparator, "<") == 0) {
    return "BGE";
  }
  else if (strcmp(comparator, "<=") == 0) {
    return "BGT";
  }
  else if (strcmp(comparator, ">") == 0) {
    return "BLE";
  }
  else if (strcmp(comparator, ">=") == 0) {
    return "BLT";
  }
  else {
    return "\0";
  }
}