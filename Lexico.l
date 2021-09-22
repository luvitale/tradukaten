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

op_and ("&&"|"AND"|"and"|"And")
op_or ("||"|"OR"|"or"|"Or")

id {letter}(_?({letter}|{digit}))*
int_constant {op_sub}?{digit}+
real_constant {digit}*\.{digit}*
string_constant \".*?\"

separator ";"

op_if ("if"|"IF"|"If"|"?<")
op_endif ("endif"|"ENDIF"|"Endif"|"EndIf"|">?")

op_while ("while"|"WHILE"|"While"|"@<")
op_endwhile ("endwhile"|"ENDWHILE"|"Endwhile"|"EndWhile"|">@")

op_dim ("dim"|"DIM"|"Dim"|"$<"|"₿<")
op_as ("as"|"AS"|"As"|">$"|">₿")

int_type ("integer"|"INTEGER"|"Integer"|"$Z"|"₿Z")
real_type ("real"|"REAL"|"Real"|"$R"|"₿R")
string_type ("string"|"STRING"|"String"|"$\""|"₿\"")

open_dec "["
close_dec "]"

dec_separator ","

open_parenthesis "("
close_parenthesis ")"

%%
{op_dim} {
  return op_dim;
}
{op_as} {
  return op_as;
}

{dec_separator} {
  return dec_separator;
}

{open_dec} {
  return open_dec;
}
{close_dec} {
  return close_dec;
}

{int_type} {
  return int_type;
}
{real_type} {
  return real_type;
}
{string_type} {
  return string_type;
}

{op_assign} {
  return op_assign;
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

{separator} {
  return separator;
}

{op_if} {
  return op_if;
}
{op_endif} {
  return op_endif;
}

{op_while} {
  return op_while;
}
{op_endwhile} {
  return op_endwhile;
}

{open_parenthesis} {
  return open_parenthesis;
}
{close_parenthesis} {
  return close_parenthesis;
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

{op_and} {
  return op_and;
}
{op_or} {
  return op_or;
}

{int_constant} {
  return int_constant;
}
{real_constant} {
  return real_constant;
}

{string_constant} {
  return string_constant;
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