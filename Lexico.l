%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "y.tab.h"

  FILE *yyin;

  int yylval;
  int lineno = 1;

  int yyerror(char *);
%}

%option noyywrap
%option yylineno

natural_digit [1-9]
digit (0|{natural_digit})
uppercase_letter [A-Z]
lowercase_letter [a-z]
letter ({uppercase_letter}|{lowercase_letter})

id {letter}(_?({letter}|{digit}))*

%%
{id} {
  printf("id/%s\n", yytext);
  return id;
}

" "
"\n" {
  ++lineno;
}
"\t"

. {
  char s[] = "invalid character: ";

  strcat(s, yytext);
  strcat(s, "\n");

  yyerror(s);
}
%%