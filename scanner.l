/*
  Coloque aqui o identificador do grupo e dos seus membros
*/
%{
#include "parser.h" //arquivo automaticamente gerado pelo bison
%}

white [ \t]+
newline [\n]
digit [0-9]
letter [a-zA-Z]
signal [+-]
number {digit}+
integer {signal}?{number}
floatamount {signal}?{number}\.{number}
boolean 'false'|'true'
char \'[.\n]\'
string \"{char}*\"
literal {integer}|{floatamount}|{boolean}|{char}|{string}
identifier {letter}+({letter}|{digit})*

/* to do: */
specialchar [\,\;\:\(\)\[\]\{\}\+\-\*\/\<\>\=\!\&\$]
/*{","}|{";"}|{":"}|{"("}|{")"}|{"["}|{"]"}|{"{"}|{"}"}|{"+"}|{"-"}|{"*"}|{"/"}|{"<"}|{">"}|{"="}|{"!"}|{"&"}|{"$"}*/
comment "/*"[.\n]*"*/"

%%
{white} { /* Do nothing */ }
{comment} { /* Do nothing */ }
{newline} { /* Do nothing */ }
{specialchar} { return (int) yytext[0]; }

{integer} { }
{floatamount} { }
{boolean} { }
{char} { }
{string} { }

{identifier} { }

'int' { }
'float' { }
'bool' { }
'char' { }
'string' { }
'if' { }
'then' { }
'else' { }
'while' { }
'do' { }
'input' { }
'output' { }
'return' { }
'const' { }
'static' { }

'\<\=' { }
'\>\=' { }
'\=\=' { }
'\!\=' { }
'\&\&' { }
'\|\|' { }

%%
