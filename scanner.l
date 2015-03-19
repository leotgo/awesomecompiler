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
white
comment
newline
specialchar 		return (int) yytext[0];
integer			return TK_LIT_INT;
floatamount		return TK_LIT_FLOAT;
'false'			return TK_LIT_FALSE;
'true'			return TK_LIT_TRUE;
char			return TK_LIT_CHAR;
string			return TK_LIT_STRING;
identifier		return TK_IDENTIFICADOR;
'int'			return TK_PR_INT;
'float'			return TK_PR_FLOAT;
'bool'			return TK_PR_BOOL;
'char'			return TK_PR_CHAR;
'string'		return TK_PR_STRING;
'if'			return TK_PR_IF;
'then'			return TK_PR_THEN;
'else'			return TK_PR_ELSE;
'while'			return TK_PR_WHILE;
'do'			return TK_PR_DO;
'input'			return TK_PR_INPUT;
'output'		return TK_PR_OUTPUT;
'return'		return TK_PR_RETURN;
'const'			return TK_PR_CONST;
'static'		return TK_PR_STATIC;
'\<\='			return TK_OC_LE;
'\>\='			return TK_OC_GE;
'\=\='			return TK_OC_EQ;
'\!\='			return TK_OC_NE;
'\&\&'			return TK_OC_AND;
'\|\|'			return TK_OC_OR;
.			return TOKEN_ERRO;

%%
