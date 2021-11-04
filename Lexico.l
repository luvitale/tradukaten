%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "y.tab.h"
  #include "ts.h"

  int lineno = 1;

  int MIN_INT = -32768;
  int MAX_INT = 32767;
  double MIN_REAL = -2147483648.0;
  double MAX_REAL = 2147483647.0;
  int MAX_STRING = 30;

  int yyerror(char *);
%}

%option noyywrap
%option yylineno

natural_digit [1-9]
digit (0|{natural_digit})
uppercase_letter [A-ZÁÉÍÓÚÑĈĜĤĴŜŬ]
lowercase_letter [a-záéíóúñĉĝĥĵŝŭ]
letter ({uppercase_letter}|{lowercase_letter})
dot "."

valid_character ({letter}|{digit}|{op_sum}|{op_sub}|{op_mult}|{op_div}|" "|[¿?¡!():=&|;."])

/*
 * Operators
 */

op_assign (":="|"<-")

op_sum "+"
op_sub "-"
op_mult "*"
op_div "/"

/*
 * Comparators
 */
op_eq (=|==)
op_lt "<"
op_le "<="
op_gt ">"
op_ge ">="
op_ne ("!="|"<>"|"~=")

op_and ("&&"|"AND"|"and"|"And")
op_or ("||"|"OR"|"or"|"Or")

/*
 * Identifier
 */

id {letter}(_?({letter}|{digit}))*

/*
 * Constants
 */

int_constant {op_sub}?{digit}+
real_constant {op_sub}?({digit}*{dot}{digit}|{digit}+{dot}{digit}*)
string_constant {open_string}{valid_character}*{close_string}

/*
 * Reserved Words
 */

op_if ("if"|"IF"|"If"|"?<")
op_else ("else"|"ELSE"|"Else"|"<:>")
op_endif ("endif"|"ENDIF"|"Endif"|"EndIf"|">?")

op_while ("while"|"WHILE"|"While"|"@<")
op_endwhile ("endwhile"|"ENDWHILE"|"Endwhile"|"EndWhile"|">@")

op_dim ("dim"|"DIM"|"Dim"|"$<"|"₿<")
op_as ("as"|"AS"|"As"|">$"|">₿")

op_display ("display"|"DISPLAY"|"Display"|"$<<"|"₿<<")
op_get ("get"|"GET"|"Get"|"$>>"|"₿>>")

/*
 * Specials
 */

fun_long ("long"|"_>")

op_in ("in"|"IN"|"In"|"@$"|"@₿")
op_do ("do"|"DO"|"Do"|"$@"|"₿@")

/*
 * Types
 */

int_type ("integer"|"INTEGER"|"Integer"|"$Z"|"₿Z")
real_type ("real"|"REAL"|"Real"|"$R"|"₿R")
string_type ("string"|"STRING"|"String"|"$\""|"₿\"")

/*
 * Groupers
 */

open_bracket "["
close_bracket "]"

open_string \"
close_string \"

open_parenthesis "("
close_parenthesis ")"

/*
 * Separators
 */

separator ";"

comma ","

/*
 * Comment
 */

comment "*/"{valid_character}*"/*"

%%
{op_dim} {
  return op_dim;
}
{op_as} {
  return op_as;
}

{op_in} {
  return op_in;
}
{op_do} {
  return op_do;
}

{op_display} {
  return op_display;
}
{op_get} {
  return op_get;
}

{comma} {
  return comma;
}

{open_bracket} {
  return open_bracket;
}
{close_bracket} {
  return close_bracket;
}

{fun_long} {
  return fun_long;
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
{op_else} {
  return op_else;
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
  int value = strtol(yytext,(char **)NULL,10);

  char msg[MAX_STRING + strlen(yytext)];

  if(value < MIN_INT) {
    sprintf(msg, "integer constant out of range: %s, min: %d", yytext, MIN_INT);

    yyerror(msg);
  }
  
  else if(value > MAX_INT) {
    sprintf(msg, "integer constant out of range: %s, max: %d", yytext, MAX_INT);

    yyerror(msg);
  }
  
  else {
    yylval.int_val = value;
  }

  return int_constant;
}
{real_constant} {
  double value = atof(yytext);

  char msg[MAX_STRING + strlen(yytext)];

  if (value < MIN_REAL || value > MAX_REAL) {
    sprintf(
      msg,
      "real constant out of range: %s, min: %f, max: %f",
      yytext, MIN_REAL, MAX_REAL
    );

    yyerror(msg);
  }

  yylval.real_val = value;

  return real_constant;
}

{string_constant} {
  int STRING_LENGTH = strlen(yytext);

  char msg[STRING_LENGTH + MAX_STRING];

  if(STRING_LENGTH > MAX_STRING) {
    sprintf(
      msg,
      "string out of range: %s, length: %d, max: %d",
      yytext, STRING_LENGTH, MAX_STRING
    );

    yyerror(msg);
  }

  yylval.str_val = yytext;

  return string_constant;
}

{id} {
  int STRING_LENGTH = strlen(yytext);

  char msg[STRING_LENGTH + MAX_STRING];

  if(STRING_LENGTH > MAX_STRING) {
    sprintf(
      msg,
      "id out of range: %s, length: %d, max: %d",
      yytext, STRING_LENGTH, MAX_STRING
    );

    yyerror(msg);
  }

  yylval.str_val = strdup(yytext);

  return id;
}

{comment} /* skip comments */

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