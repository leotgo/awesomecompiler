/* Grupo: Knights of the Hash Table
Integrantes: Alex Gliesch, Leonardo Tagliaro, Marina Fortes Rey */

%{

#include "cc_dict.h"
#include "parser.h" /* arquivo automaticamente gerado pelo bison */
#include "cc_misc.h" /* for the recognize_token function. */
int lineCounter = 1;

%}

%x comment
white [ \t]+
digit [0-9]
letter [a-zA-Z_]
signal [+-]
integer {digit}+
floatamount {integer}\.{integer}
boolean 'false'|'true'
char \'(.)\'
string \"[^"]*\"
literal {integer}|{floatamount}|{boolean}|{char}|{string}
identifier {letter}+({letter}|{digit})*

/* to do: */
specialchar [,|;|:|(|)|\[|\]|{|}|+|\-|*|/|<|>|=|!|&|$]

%%
{white}			;
"/*" 			BEGIN(comment);
<comment>.   	;
<comment>\n     ;lineCounter++;
<comment>"*"+"/"  BEGIN(INITIAL);
"//".*			;
"\n"		;lineCounter++;
"false"			return recognize_token(TK_LIT_FALSE);
"true"			return recognize_token(TK_LIT_TRUE);
"int"			return recognize_token(TK_PR_INT);
"float"			return recognize_token(TK_PR_FLOAT);
"bool"			return recognize_token(TK_PR_BOOL);
"char"			return recognize_token(TK_PR_CHAR);
"string"		return recognize_token(TK_PR_STRING);
"if"			return recognize_token(TK_PR_IF);
"then"			return recognize_token(TK_PR_THEN);
"else"			return recognize_token(TK_PR_ELSE);
"while"			return recognize_token(TK_PR_WHILE);
"do"			return recognize_token(TK_PR_DO);
"input"			return recognize_token(TK_PR_INPUT);
"output"		return recognize_token(TK_PR_OUTPUT);
"return"		return recognize_token(TK_PR_RETURN);
"const"			return recognize_token(TK_PR_CONST);
"static"		return recognize_token(TK_PR_STATIC);
"<="			return recognize_token(TK_OC_LE);
">="			return recognize_token(TK_OC_GE);
"=="			return recognize_token(TK_OC_EQ);
"!="			return recognize_token(TK_OC_NE);
"&&"			return recognize_token(TK_OC_AND);
"||"			return recognize_token(TK_OC_OR);
{integer}			return recognize_token(TK_LIT_INT);
{floatamount}		return recognize_token(TK_LIT_FLOAT);
{char}			return recognize_token(TK_LIT_CHAR);
{string}			return recognize_token(TK_LIT_STRING);
{specialchar}	return recognize_token((int) yytext[0]);
{identifier}		return recognize_token(TK_IDENTIFICADOR);
.				return TOKEN_ERRO;
%%
