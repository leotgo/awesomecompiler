#ifndef __TREE_H
#define __TREE_H

#include "cc_ast.h"
#include "cc_dict.h"
#include <stdarg.h>

typedef struct comp_tree_t {
	int type; /* the type of this node. see cc_ast.h for reference */

	comp_dict_item_t* sym_table_ptr; /* if the node type is AST_LITERAL or 
									 AST_IDENTIFICADOR, this will point to the
									 entry on the symbols table correspondent to
									 that symbol. otherwise, it is NULL. */

	int num_children; /* the number of children pertaining to that node. */

	struct comp_tree_t** children; /* a list of node pointers to the children
								   of this node. if num_children == 0, this
								   pointer is NULL. */

	struct comp_tree_t* next; /* if the node is part of a list (for instance a
							  block of commands or argument list), it has a 
							  pointer to the next node of that list.

							  if it is the last element of a list, or it is not
							  part of a list at all, this pointer will be NULL.
							  
							  in the .dot format, this node will be drawn as 
							  the last child on the list. */
} comp_tree_t;

/* the global syntax tree, which will be declared in cc_tree.c.  */
extern comp_tree_t* global_syntax_tree;

/* deallocates a tree node, including its children and next, and frees the 
 * memory from it. */
void free_tree_node(comp_tree_t* t);

/* creates and returns an (dynamically allocated) ast node, adds its children 
 * (specified in the variable argument list) as children nodes.
 * 
 * it the node type is AST_LITERAL or AST_IDENTIFICADOR, the first child is 
 * interpreted as a
 * 
 * node is a literal or identifier, sets a pointer to the symbols table.
 * 
 * assumes 'type' is indeed a valid type, definedin cc_ast.h */
comp_tree_t* ast_create(int type, ...);

/* the ast_createv is an internal helper function that takes a va_list instead
 * of a ..., so it can pass the va_list as argument to other functions.
 * 
 * ps: i may change/remove this later, but, for now, i'll leave it like 
 * this. */
comp_tree_t* ast_createv(int type, va_list args);

/* Adds 'next' as a next of 'first' in the current ast list.
 * 
 * ListaDeExpressoes
 * | Expressao = { return eval($1) } 
 * | Expressao , ListaDeExpressoes = { return ast_list(eval($1), eval($2); }
 * */
comp_tree_t* ast_list(comp_tree_t* first, comp_tree_t* next) {
	if (first != NULL && next != NULL)
		first->next = next;
	return first;
}

/* allocates num_children children for the node t, and adds the arguments in 
 * 'va_list args' as children. it is expected that args has at least 
 * num_children elements, and that their values are comp_tree_t* (ast nodes). */
void ast_create_children(comp_tree_t* t, int num_children, va_list args);

void ast_generate_dot_graph(comp_tree_t* t);

#endif

/* - porque 'Inteiro' é uma regra separada?
 * 
 * - porque agente reconhece tanto & quanto &&, e | quanto ||? alguma diferença
 * entre elas?
 * 
 * - não entendi a regra de 'Entrada'. no pdf da etapa 3, diz que input tem só
 * 1 filho: o identificador pra onde o input vai. pq então tem 2 expressões na 
 * regra? 
 * 
 * - qual a diferença entre o 'Argumentos' e o 'ArgumentosNaoVazio'? eles são
 * exatamente iguais...
 * 
 * - Em 'Declaracoes', acho que vai dar problema. Porque a declaração de 
 * variáveis não deve constar na AST, então não posso colocar elas na lista...
 * ou agente ignora elas, ou então coloca e depois faz um pós-processamento da
 * árvore pra remover elas, o que funcionaria mas acho meio chinelagem
 * */

/*  
 * ----------------------------------------------------------------------------
 * ListaDeExpressoes:
 * | Expressao {  $$ = $1; }
 * | Expressao ',' ListaDeExpressoes { $$ = ast_list($1, $3); }
 * 
 * ----------------------------------------------------------------------------
 * 
 * Atribuicao:
 * | Identificador '=' Expressao { 
 *		$$ = ast_create(AST_ATRIBUICAO, $1, $3); }
 * 
 * Neste segundo caso acho melhor criar uma nova regra na gramática pra vetor 
 * indexado...
 * 
 * | Identificador '[' Expressao ']' '=' Expressao { 
 *		$$ = ast_create(AST_ATRIBUICAO, 
 *						ast_create(AST_VETOR_INDEXADO, $1, $3), $6); } 
 *						  
 *-----------------------------------------------------------------------------
 *	
 *	Saida:
 *	| TK_PR_OUTPUT ListaDeExpressoes {
 *		$$ = $2; }
 *
 * ----------------------------------------------------------------------------
 * 
 * SequenciaDeComandos:
 *
 *   Comando { $$ = $1; }
 * | Comando ';' { $$ = $1; }
 *  | Comando ';' SequenciaDeComandos { $$ = ast_list($1, $3); }
 * ;
 * 
 * CtrlFluxoIf:
 * TK_PR_IF Expressao TK_PR_THEN Comando { $$ = ast_create(AST_IF_ELSE, $2, $4, NULL); }
 * | TK_PR_IF Expressao TK_PR_THEN Comando TK_PR_ELSE Comando { $$ = ast_create(AST_IF_ELSE, $2, $4, $6); }
 * | TK_PR_IF TK_PR_THEN Comando {yyerror("Erro: Comando de Fluxo IF sem condicao ");YYERROR;}
 * ;
 */