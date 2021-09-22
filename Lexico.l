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

op_assign (":="|"<-")

op_sum "+"
op_sub "-"
op_mult "*"
op_div "/"

op_eq ("="|"==")
op_lt "<"
op_le "<="
op_gt ">"
op_ge ">="
op_ne ("!="|"<>"|"~=")

id {letter}(_?({letter}|{digit}))*
int_constant {op_sub}?{digit}+

separator_assign ";"

%%
{op_assign} {
  return op_assign;
}

{semicolon} {
  return separator_assign;
}

{op_sum} {
  return op_sum;
}
{op_sub} {
  return op_sub;
}
{op_mult} {
  return op_mult;
}
{op_div} {
  return op_div;
}

{op_eq} {
  return op_eq;
}
{op_lt} {
  return op_lt;
}
{op_le} {
  return op_le;
}
{op_gt} {
  return op_gt;
}
{op_ge} {
  return op_ge;
}
{op_ne} {
  return op_ne;
}

{int_constant} {
  return int_constant;
}

{id} {
  return id;
}

" "
"\n" {
  ++lineno;
}
"\t"
"\n\t"
"\r\n"

. {
  char s[] = "invalid character: ";

  strcat(s, yytext);
  strcat(s, "\n");

  yyerror(s);
}
%%