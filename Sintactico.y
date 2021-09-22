%{
  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include "y.tab.h"

  #define COLOR_RED "\033[1;31m"
  #define COLOR_RESET "\033[0m"

  int lineno;

  int yylval;
  int yystopparser = 0;

  FILE *yyin;

  char *yyltext;
  char *yytext;

  int yylex();
  int yyerror(char *);
%}

%token id;
%token int_constant;

%token op_assign;
%token op_sum op_sub op_mult op_div;
%token separator_assign;

%token op_eq op_lt op_le op_gt op_ge op_ne;

%%
PROGRAM: CODE;

CODE: CODE BLOCK | BLOCK;

BLOCK: ASSIGNMENT separator_assign;

ASSIGNMENT: id op_assign EXPRESSION
          | id op_assign ASSIGNMENT;

EXPRESSION: EXPRESSION op_sum FACTOR
          | EXPRESSION op_sub FACTOR
          | FACTOR;

FACTOR: FACTOR op_mult TERM
      | FACTOR op_div TERM
      | TERM;

TERM: id | int_constant;
%%

int main(int argc,char *argv[]) {
  const char* filename = argv[1];
  FILE* arg_file = fopen(filename, "rt");

  if (arg_file == NULL ) {
    printf("File cannot be opened: %s\n", filename);
    printf("Using standard input\n\n");
  }
  else {
    yyin = arg_file;
  }

  yyparse();

  fclose(yyin);

  return EXIT_SUCCESS;
}

int yyerror(char *error) {
  fprintf(stderr, COLOR_RED "\nline %d: %s\n" COLOR_RESET, lineno, error);
  fclose(yyin);
  exit(1);
}