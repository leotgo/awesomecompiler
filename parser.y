/*
  Coloque aqui o identificador do grupo e dos seus membros
*/
%{
#include <stdio.h>
%}

/* Declaração dos tokens da linguagem */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_STATIC
%token TK_PR_CONST
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%start Programa

%%
/* Regras (e ações) da gramática */

Static:
		  TK_PR_STATIC

Const:
		  TK_PR_CONST

Tipo:
		  TK_PR_INT
		| TK_PR_FLOAT
		| TK_PR_BOOL
		| TK_PR_CHAR
		| TK_PR_STRING

Inteiro:
		  TK_LIT_INT

Literal:
		  Inteiro
		| TK_LIT_FLOAT
		| TK_LIT_TRUE
		| TK_LIT_FALSE
		| TK_LIT_CHAR
		| TK_LIT_STRING

Identificador:
		  TK_IDENTIFICADOR

Comparador:
		  TK_OC_LE
		| TK_OC_GE
		| TK_OC_EQ
		| TK_OC_NE
		| TK_OC_AND
		| TK_OC_OR

MathOp:
		  '+'
		| '-'
		| '*'
		| '/'

BinaryOp:
		  '&'
		| '|'

Operador:
		  MathOp
		| BinaryOp

Expressao:
		'(' Expressao ')'
		| Expressao Operador Expressao
		| Expressao Comparador Expressao
		| Literal
		| Identificador
		| Identificador '[' Expressao ']'
		| ChamadaDeFuncao

ListaDeExpressoes:
		  Expressao
		| Expressao ',' ListaDeExpressoes

Atribuicao:
		  Identificador '=' Expressao
		| Identificador '[' Expressao ']' '=' Expressao

Entrada:
		  TK_PR_INPUT Expressao '=' '>' Expressao

Saida:
		  TK_PR_OUTPUT ListaDeExpressoes

Variavel:
		  Tipo Identificador

Vetor:
		Variavel '[' Inteiro ']'

DeclVariavelGlobal:
		  Variavel ';'
		| Vetor ';'
		| Static Variavel ';'
		| Static Vetor ';'

VariavelLocal:
		  Variavel
		| Const Variavel
		| Static Variavel
		| Const Static Variavel

DeclVariavelLocal:
		  VariavelLocal
		| VariavelLocal '<' '=' Literal
		| VariavelLocal '<' '=' Identificador

Retorno:
		  TK_PR_RETURN Expressao

Parametro:
		  Variavel
		| Const Variavel

Parametros:

		  %empty
		| Parametro
		| Parametro ',' ParametrosNaoVazio

ParametrosNaoVazio:
		  Parametro
		| Parametro ',' ParametrosNaoVazio

DeclFuncao:
		  Tipo Identificador '(' Parametros ')' BlocoDeComandos
		| Static Tipo Identificador '(' Parametros ')' BlocoDeComandos

ArgumentosNaoVazio:

		%empty		
		| Expressao
		| Expressao ',' ArgumentosNaoVazio

Argumentos:

		  Expressao
		| Expressao ',' ArgumentosNaoVazio

ChamadaDeFuncao:
		  Identificador '(' Argumentos ')'

Comando:
		%empty
		| ';'
		| DeclVariavelLocal ';'
		| Atribuicao ';'
		| Entrada ';'
		| Saida ';'
		| Retorno ';'
		| BlocoDeComandos 
		| ChamadaDeFuncao ';'
		| ControleDeFluxo
		
SequenciaDeComandos:

		  Comando 
		| Comando SequenciaDeComandos

BlocoDeComandos:
		  '{' SequenciaDeComandos '}'

CtrlFluxoIf:
		  TK_PR_IF Expressao TK_PR_THEN Comando
		| TK_PR_IF Expressao TK_PR_THEN Comando TK_PR_ELSE Comando

CtrlFluxoWhile:
		  TK_PR_WHILE Expressao TK_PR_DO Comando

CtrlFluxoDoWhile:
		  TK_PR_DO Comando TK_PR_WHILE Expressao

ControleDeFluxo:
		  CtrlFluxoIf
		| CtrlFluxoWhile
		| CtrlFluxoDoWhile

DeclaracoesGlobais:
		  DeclVariavelGlobal
		| DeclVariavelGlobal DeclaracoesGlobais

DeclaracoesFuncoes:
		  DeclFuncao
		| DeclFuncao DeclaracoesFuncoes

DeclaracoesOpcionais:

		  %empty
		| DeclaracoesGlobais
		| DeclaracoesFuncoes
		| DeclaracoesGlobais DeclaracoesOpcionais
		| DeclaracoesFuncoes DeclaracoesOpcionais

Programa:
		  DeclaracoesOpcionais
/*
	Itens:
	2.1 Declarações de variáveis globais 	- OK
	2.2 Definição de funções 				- OK
	2.3 Comandos
		-> Declaração de variável local		- OK
		-> Atribuição						- OK
		-> Entrada							- OK
		-> Saída							- OK
		-> Retorno							- OK
		-> Bloco de Comandos 				- OK
		-> Fluxo de Controle 				- OK
		-> Comando vazio 					- OK
		-> Chamada de função				- OK
	2.4 Bloco de Comandos 					- OK
	2.5 Expressões aritméticas e lógicas  	- OK
	2.6 Comandos de fluxo de controle 		- OK
*/

%%
