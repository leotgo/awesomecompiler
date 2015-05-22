
%{
#include <stdio.h>

#include "cc_context.h"
#include "cc_tree.h"
#include "cc_dict.h" 
#include "cc_error.h"
#include "cc_gencode.h"
  
%}

/* Declaração dos tokens da linguagem */

%token <valor_simbolo_lexico> TK_PR_INT
%token <valor_simbolo_lexico> TK_PR_FLOAT
%token <valor_simbolo_lexico> TK_PR_BOOL
%token <valor_simbolo_lexico> TK_PR_CHAR
%token <valor_simbolo_lexico> TK_PR_STRING
%token <valor_simbolo_lexico> TK_PR_IF
%token <valor_simbolo_lexico> TK_PR_THEN
%token <valor_simbolo_lexico> TK_PR_ELSE
%token <valor_simbolo_lexico> TK_PR_WHILE
%token <valor_simbolo_lexico> TK_PR_DO
%token <valor_simbolo_lexico> TK_PR_INPUT
%token <valor_simbolo_lexico> TK_PR_OUTPUT
%token <valor_simbolo_lexico> TK_PR_RETURN
%token <valor_simbolo_lexico> TK_PR_STATIC
%token <valor_simbolo_lexico> TK_PR_CONST
%token <valor_simbolo_lexico> TK_OC_LE
%token <valor_simbolo_lexico> TK_OC_GE
%token <valor_simbolo_lexico> TK_OC_EQ
%token <valor_simbolo_lexico> TK_OC_NE
%token <valor_simbolo_lexico> TK_OC_AND
%token <valor_simbolo_lexico> TK_OC_OR
%token <valor_simbolo_lexico> TK_LIT_INT
%token <valor_simbolo_lexico> TK_LIT_FLOAT
%token <valor_simbolo_lexico> TK_LIT_FALSE
%token <valor_simbolo_lexico> TK_LIT_TRUE
%token <valor_simbolo_lexico> TK_LIT_CHAR
%token <valor_simbolo_lexico> TK_LIT_STRING
%token <valor_simbolo_lexico> TK_IDENTIFICADOR
%token <valor_simbolo_lexico> TOKEN_ERRO

/* declaring the type of each nonterminal symbol (all are ast nodes) */
%type <ast_node> Programa
%type <ast_node> Declaracoes
%type <ast_node> Declaracao
%type <ast_node> ControleDeFluxo
%type <ast_node> CtrlFluxoWhile
%type <ast_node> CtrlFluxoDoWhile
%type <ast_node> CtrlFluxoIf
%type <ast_node> BlocoDeComandos
%type <ast_node> SequenciaDeComandos
%type <ast_node> Comando
%type <ast_node> ChamadaDeFuncao
%type <ast_node> Argumentos
%type <ast_node> ArgumentosNaoVazio
%type <ast_node> DeclFuncao
%type <ast_node> Retorno
%type <ast_node> Saida
%type <ast_node> Entrada
%type <ast_node> Atribuicao
%type <ast_node> ListaDeExpressoes
%type <ast_node> Expressao
%type <ast_node> Identificador
%type <ast_node> Literal
%type <ast_node> Inteiro
%type <ast_node> BlocoDeComandosFuncao
%type <ast_node> AcessoVetor
%type <ast_node> DimensoesVetor
%type <valor_simbolo_lexico> Variavel
%type <valor_simbolo_lexico> Tipo
//%type <valor_simbolo_lexico> Inteiro
%type <valor_simbolo_lexico> Parametro
%type <valor_simbolo_lexico> Parametros
%type <valor_simbolo_lexico> ParametrosNaoVazio


%union 
{
    struct comp_dict_item_t* valor_simbolo_lexico;
	struct comp_tree_t* ast_node;
}

%left TK_OC_OR 
%left TK_OC_AND
%nonassoc TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE '<' '>'
%left '+' '-'
%left '*' '/'
%left '&' '|'
%left '!'
%right TK_PR_THEN TK_PR_ELSE

%start Programa

%%
/* Regras (e ações) da gramática */

Static:
		  TK_PR_STATIC { }
		;

Const:
		  TK_PR_CONST { }
		;

Tipo:
		  TK_PR_INT { $$ = $1; }
		| TK_PR_FLOAT { $$ = $1; }
		| TK_PR_BOOL { $$ = $1; }
		| TK_PR_CHAR { $$ = $1; }
		| TK_PR_STRING { $$ = $1; }
		;

Inteiro:
		  TK_LIT_INT { $$ = ast_create(AST_LITERAL, $1); }
		;

Literal:
		  Inteiro { $$ = $1; }
		| TK_LIT_FLOAT { $$ = ast_create(AST_LITERAL, $1); }
		| TK_LIT_TRUE { $$ = ast_create(AST_LITERAL, $1); }
		| TK_LIT_FALSE { $$ = ast_create(AST_LITERAL, $1); }
		| TK_LIT_CHAR { $$ = ast_create(AST_LITERAL, $1); }
		| TK_LIT_STRING { $$ = ast_create(AST_LITERAL, $1); }
		;

Identificador:
		  TK_IDENTIFICADOR { $$ = ast_create(AST_IDENTIFICADOR, $1); }
		;

Expressao:
		'(' Expressao ')' { $$ = $2; }
		| '+' Expressao { $$ = $2; }
		| '-' Expressao { $$ = ast_create(AST_ARIM_INVERSAO, $2); }	//todo
		| '!' Expressao { $$ = ast_create(AST_LOGICO_COMP_NEGACAO, $2); }	//todo
		| Literal { $$ = $1; }
		| Identificador { $$ = $1; }
		| Identificador '[' AcessoVetor ']' {  $1->vector_dimensions = $3->vector_dimensions;
			$$ = ast_create(AST_VETOR_INDEXADO, $1, $3); check_vector_dimensions_number($1); }
		| ChamadaDeFuncao { $$ = $1; }
		| Expressao '+' Expressao { 
			$$ = ast_create(AST_ARIM_SOMA, $1, $3); }
		| Expressao '-' Expressao { 
			$$ = ast_create(AST_ARIM_SUBTRACAO, $1, $3); }
		| Expressao '*' Expressao { 
			$$ = ast_create(AST_ARIM_MULTIPLICACAO, $1, $3); } 
		| Expressao '/' Expressao { 
			$$ = ast_create(AST_ARIM_DIVISAO, $1, $3); }
		| Expressao '&' Expressao { 
			$$ = ast_create(AST_LOGICO_E, $1, $3); }
		| Expressao '|' Expressao { 
			$$ = ast_create(AST_LOGICO_OU, $1, $3); }
		| Expressao '>' Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_G, $1, $3); }
		| Expressao '<' Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_L, $1, $3); }
		| Expressao TK_OC_OR Expressao { 
			$$ = ast_create(AST_LOGICO_OU, $1, $3); }
		| Expressao TK_OC_AND Expressao { 
			$$ = ast_create(AST_LOGICO_E, $1, $3); }
		| Expressao TK_OC_NE Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_DIF, $1, $3); }
		| Expressao TK_OC_EQ Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_IGUAL, $1, $3); }
		| Expressao TK_OC_LE Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_LE, $1, $3); }
		| Expressao TK_OC_GE Expressao { 
			$$ = ast_create(AST_LOGICO_COMP_GE, $1, $3); }
		| Literal '[' Inteiro ']' {
			yyerror("Erro: Identificador invalido"); YYERROR; }	
		;

ListaDeExpressoes:
		  Expressao { $$ = $1; }		  
		| Expressao ',' ListaDeExpressoes { $$ = ast_list($1, $3); }
		;

Atribuicao:
		  Identificador '=' Expressao { $$ = ast_create(AST_ATRIBUICAO, $1, $3); get_type($3, PURPOSE_NORMAL);  }
		| Identificador '[' AcessoVetor ']' '=' Expressao { $1->vector_dimensions = $3->vector_dimensions;
															$$ = ast_create(AST_ATRIBUICAO, ast_create(AST_VETOR_INDEXADO, $1, $3), $6); 
															get_type($1, PURPOSE_VECTOR);}
		| Literal '=' Expressao { yyerror("Erro: Identificador invalido"); YYERROR; }	
		;

Entrada:
		  TK_PR_INPUT Expressao '=' '>' Expressao { 
		  $$ = ast_create(AST_INPUT, $2,$5); }
		;

Saida:
		  TK_PR_OUTPUT ListaDeExpressoes { $$ = ast_create(AST_OUTPUT, $2); }
		;

Variavel:
		  Tipo TK_IDENTIFICADOR { context_add_identifier_to_current($2->token,$1->token_type, PURPOSE_NORMAL, 0, NULL); $$ = $1;}
		;

AcessoVetor:

		  Expressao						{ type_check_vector_dimension($1); $$ = $1; $$->vector_dimensions = 1; }
		| Expressao ',' AcessoVetor		{ $$->vector_dimensions = 1 + $3->vector_dimensions; $$ = ast_list($1, $3); }
		;

DimensoesVetor:
	
		  Inteiro						{ $$ = $1; $$->vector_dimensions = 1; $$->vector_dimension_sizes[$$->vector_dimensions - 1] = * ((int*) ($1->sym_table_ptr->value)); }
		| Inteiro ',' DimensoesVetor	{ 
				$$->vector_dimensions = 1 + $3->vector_dimensions; 
				memcpy($$->vector_dimension_sizes, $3->vector_dimension_sizes, sizeof($$->vector_dimension_sizes));
				$$->vector_dimension_sizes[$$->vector_dimensions - 1] = * ((int*) ($1->sym_table_ptr->value)); 
			}
		;

Vetor:
		Tipo TK_IDENTIFICADOR '[' DimensoesVetor ']' 	{ 
			context_add_identifier_to_current($2->token, $1->token_type, PURPOSE_VECTOR, $4->vector_dimensions, $4->vector_dimension_sizes);			
		}
		;

DeclVariavelGlobal: 
		  Variavel ';' { }
		| Vetor ';' { }
		| Static Variavel ';' { }
		| Static Vetor ';' { }
		;

VariavelLocal:
		  Variavel { }
		| Vetor		{ }
		| Const Variavel { }
		| Const Vetor	 { }
		| Static Vetor	 { }
		| Static Variavel { }
		| Static Const Vetor	{ }
		| Static Const Variavel { }
		;

DeclVariavelLocal:
		  VariavelLocal { }
		| VariavelLocal TK_OC_LE Literal { }
		| VariavelLocal TK_OC_LE TK_IDENTIFICADOR { }
		;

Retorno:
		  TK_PR_RETURN Expressao { $$ = ast_create(AST_RETURN, $2); }
		;

Parametro:
		  Variavel { $$ = $1;}
		| Const Variavel { $$ = $2;}
		;

Parametros:
 		  { $$ = NULL; }
		| Parametro { $$->params_list = type_list_Add(NULL, $1->token_type);}
		| Parametro ',' ParametrosNaoVazio { $$->params_list = type_list_Add(type_list_Add($3->params_list, $3->token_type), $$->token_type); }
		;

ParametrosNaoVazio:
		  Parametro { $$->params_list = type_list_Add(NULL, $1->token_type); }
		| Parametro ',' ParametrosNaoVazio { $$->params_list = type_list_Add(type_list_Add($3->params_list, $3->token_type), $$->token_type); }
		;

DeclFuncao:
		  Tipo TK_IDENTIFICADOR '(' Parametros ')' { context_add_function_to_current($2->token,$1->token_type, $4); } BlocoDeComandosFuncao { 	$$ = ast_create(AST_FUNCAO, $2, $7); }
		| Static Tipo TK_IDENTIFICADOR '(' Parametros  ')' { context_add_function_to_current($3->token,$2->token_type, $5); } BlocoDeComandosFuncao { $$ = ast_create(AST_FUNCAO, $3, $8); }
		| Tipo TK_IDENTIFICADOR '(' Parametros ')' ';'	BlocoDeComandosFuncao {	yyerror(" Erro: definicao de funcao seguida de ; "); YYERROR; }
		| Static Tipo TK_IDENTIFICADOR '(' Parametros ')' ';' BlocoDeComandosFuncao { yyerror(" Erro: definicao de funcao seguida de ; "); YYERROR; }
		;

ArgumentosNaoVazio:
		  Expressao { $$ = $1; $$->expectedTypes = type_list_Add(NULL, type_inference($1));}
		| Expressao ',' ArgumentosNaoVazio { $$ = ast_list($1, $3);$$->expectedTypes = type_list_Add($$->expectedTypes, type_inference($1));}
		;

Argumentos:
	      { $$ = NULL; }
		| Expressao { $$->expectedTypes = type_list_Add(NULL, type_inference($1));}
		| Expressao ',' ArgumentosNaoVazio { $$ = ast_list($1, $3); $$->expectedTypes = type_list_Add($$->expectedTypes, type_inference($1));}
		;

ChamadaDeFuncao:
		 Identificador '(' Argumentos ')' { $$ = ast_create(AST_CHAMADA_DE_FUNCAO, $1, $3); type_check_function_call($1, $3);}
		;

Comando:
		  ';' { $$ = NULL; }
		| DeclVariavelLocal { $$ = NULL; }
		| Atribuicao { }
		| Entrada { }
		| Saida { }
		| Retorno { }
		| BlocoDeComandos { }
		| ChamadaDeFuncao { }
		| ControleDeFluxo { }
		;

SequenciaDeComandos:
		 Comando { }
		| Comando ';' { }
		| Comando ';' SequenciaDeComandos { $$ = ast_list($1, $3); }
		;

BlocoDeComandosFuncao:
		  '{' { context_push_new(); } SequenciaDeComandos { type_check($3);context_pop(); } '}' { $$ = $3; }
		| '{' '}' { $$ = NULL; }
		;

BlocoDeComandos:
		  '{' { context_push_new(); } SequenciaDeComandos { type_check($3); context_pop(); } '}' { $$ = ast_create(AST_BLOCO, $3); }
		| '{' '}' { $$ = ast_create(AST_BLOCO, NULL); }
		;

CtrlFluxoIf:
		  TK_PR_IF Expressao TK_PR_THEN Comando { 
		  	$$ = ast_create(AST_IF_ELSE, $2, $4, NULL); }
		| TK_PR_IF Expressao TK_PR_THEN Comando TK_PR_ELSE Comando { 
			$$ = ast_create(AST_IF_ELSE, $2, $4, $6); }
		| TK_PR_IF TK_PR_THEN Comando { 
			yyerror("Erro: Comando de Fluxo IF sem condicao ");YYERROR; }
		;

CtrlFluxoWhile:
		  TK_PR_WHILE Expressao TK_PR_DO Comando { 
		  	$$ = ast_create(AST_WHILE_DO, $2, $4); }
		;

CtrlFluxoDoWhile:
		  TK_PR_DO Comando TK_PR_WHILE Expressao { $$ = ast_create(
		  	AST_DO_WHILE, $2, $4); }
		;

ControleDeFluxo:
		  CtrlFluxoIf { }
		| CtrlFluxoWhile { }
		| CtrlFluxoDoWhile { }
		;

Declaracao:
		DeclVariavelGlobal { $$ = NULL; }
		| DeclFuncao { type_check($1); }
		;

Declaracoes:
		Declaracao { }
		| Declaracao Declaracoes { $$ = ast_list($1, $2); }
		;

Programa:
		   { $$ = ast_create(AST_PROGRAMA, NULL); ast_generate_dot_graph(global_syntax_tree); 
			context_calc_addr(); generate_code(global_syntax_tree, NULL); exit(IKS_SUCCESS); }
		| Declaracoes { $$ = ast_create(AST_PROGRAMA, $1); ast_generate_dot_graph(global_syntax_tree); 
			context_calc_addr(); 
			generate_code(global_syntax_tree, NULL);
			 free_value_pool(); 
			exit(IKS_SUCCESS);}
		;
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

