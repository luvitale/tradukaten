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

%%
PROGRAM: CODE;

CODE: CODE LINE | LINE;

LINE: EXPRESSION;

EXPRESSION: id;
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