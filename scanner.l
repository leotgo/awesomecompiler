/*
  Coloque aqui o identificador do grupo e dos seus membros
*/
%{
#include "parser.h" //arquivo automaticamente gerado pelo bison
%}

white [ \t]+
digit [0-9]
letter [a-zA-Z]
signal [+-]
number {digit}+
integer {signal}?{number}
floatamount {signal}?{number}\.{number}
boolean {"false"}|{"true"}
identifier {letter}+({letter}|{digit})*

/* to do: */
specialchar {","}|{";"}|{":"}|{"("}|{")"}|{"["}|{"]"}|{"{"}|{"}"}|{"+"}|{"-"}|{"*"}|{"/"}|{"<"}|{">"}|{"="}|{"!"}|{"&"}|{"$"}
comment {"/*"}[.\n]*{"*/"}

%%

/* Regras e acoes */

{white} { }
{specialchar} { /* retornar codigo ASCII */ }

/* to do - reserved keywords */
{"int"} { }
{"float"} { }
{"bool"} { }
{"char"} { }
{"string"} { }
{"if"} { }
{"then"} { }
{"else"} { }
{"while"} { }
{"do"} { }
{"input"} { }
{"output"} { }
{"return"} { }
{"const"} { }
{"static"} { }


/* also to do - comparers */
{"<="} { }
{">="} { }
{"=="} { }
{"!="} { }
{"&&"} { }
{"||"} { }

%%

/* Funcoes opcionais
  campo opcional nao utilizado */
