/* Grupo: Knights of the Hash Table
Integrantes: Alex Gliesch, Leonardo Tagliaro, Marina Fortes Rey */

%{
#include "parser.h" /* arquivo automaticamente gerado pelo bison */
#include "cc_misc.h" /* for the recognize_token function. */
int lineCounter = 1;

%}

%x comment
white [ \t]+
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
specialchar [,|;|:|(|)|\[|\]|{|}|+|\-|*|/|<|>|=|!|&|$]
/*specialchar [\,\;\:\(\)\[\]\{\}\+\-\*\/\<\>\=\!\&\$]*/
/*{","}|{";"}|{":"}|{"("}|{")"}|{"["}|{"]"}|{"{"}|{"}"}|{"+"}|{"-"}|{"*"}|{"/"}|{"<"}|{">"}|{"="}|{"!"}|{"&"}|{"$"}*/

%%
{white}			;
"/*" 			BEGIN(comment);
<comment>.   	;
<comment>\n     ;lineCounter++;
<comment>"*"+"/"  BEGIN(INITIAL);
"//".*			;
"\n"		;lineCounter++;
"false"			return recognize_token(yytext, TK_LIT_FALSE);
"true"			return recognize_token(yytext, TK_LIT_TRUE);
"int"			return recognize_token(yytext, TK_PR_INT);
"float"			return recognize_token(yytext, TK_PR_FLOAT);
"bool"			return recognize_token(yytext, TK_PR_BOOL);
"char"			return recognize_token(yytext, TK_PR_CHAR);
"string"		return recognize_token(yytext, TK_PR_STRING);
"if"			return recognize_token(yytext, TK_PR_IF);
"then"			return recognize_token(yytext, TK_PR_THEN);
"else"			return recognize_token(yytext, TK_PR_ELSE);
"while"			return recognize_token(yytext, TK_PR_WHILE);
"do"			return recognize_token(yytext, TK_PR_DO);
"input"			return recognize_token(yytext, TK_PR_INPUT);
"output"		return recognize_token(yytext, TK_PR_OUTPUT);
"return"		return recognize_token(yytext, TK_PR_RETURN);
"const"			return recognize_token(yytext, TK_PR_CONST);
"static"		return recognize_token(yytext, TK_PR_STATIC);
"<="			return recognize_token(yytext, TK_OC_LE);
">="			return recognize_token(yytext, TK_OC_GE);
"=="			return recognize_token(yytext, TK_OC_EQ);
"!="			return recognize_token(yytext, TK_OC_NE);
"&&"			return recognize_token(yytext, TK_OC_AND);
"||"			return recognize_token(yytext, TK_OC_OR);
{integer}			return recognize_token(yytext, TK_LIT_INT);
{floatamount}		return recognize_token(yytext, TK_LIT_FLOAT);
{specialchar}	return recognize_token(yytext, (int) yytext[0]);
{char}			return recognize_token(yytext, TK_LIT_CHAR);
{string}			return recognize_token(yytext, TK_LIT_STRING);
{identifier}		return recognize_token(yytext, TK_IDENTIFICADOR);
%%
