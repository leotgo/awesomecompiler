#include "cc_tree.h"
#include "cc_gv.h"
#include "cc_context.h"
#include <stdio.h>

comp_tree_t* global_syntax_tree = NULL;

typedef struct tree_node_pool
{
	comp_tree_t* node;
	struct tree_node_pool* next;
} tree_node_pool;

tree_node_pool* nodes_pool = NULL;
tree_node_pool* current_pool_node = NULL;

void free_tree_node(comp_tree_t* t) {
	if (t == NULL) return;
	
	int i;
	for (i = 0; i < t->num_children; ++i)
		if (t->children[i] != NULL)
			free_tree_node(t->children[i]);

	if (t->children)
		free(t->children);

	if (t->next) 
		free_tree_node(t->next);

	if (t->expectedTypes)
		type_list_free(t->expectedTypes);

	free(t);
}

void free_node_pool()
{
	current_pool_node = nodes_pool;
	while(current_pool_node != NULL)
	{
		tree_node_pool* freed_node = current_pool_node;
		current_pool_node = current_pool_node->next;
		free_tree_node(freed_node->node);
		free(freed_node);
	}
	nodes_pool = NULL;
}

comp_tree_t* ast_create(int type, ...) {
	va_list args;
	va_start(args, type);
	comp_tree_t* ans = ast_createv(type, args);
	
	if(nodes_pool == NULL)
	{
		nodes_pool = (tree_node_pool*)malloc(sizeof(tree_node_pool));
		nodes_pool->node = ans;
		nodes_pool->next = NULL;
		current_pool_node = nodes_pool;
	}
	else
	{
		current_pool_node->next = (tree_node_pool*)malloc(sizeof(tree_node_pool));
		current_pool_node = current_pool_node->next;
		current_pool_node->node = ans;
		current_pool_node->next = NULL;
	}

	va_end(args);

	return ans;	
}

comp_tree_t* ast_createv(int type, va_list args) {
	comp_tree_t* t = (comp_tree_t*) malloc(sizeof(comp_tree_t));
	t->type = type;
	t->num_children = 0;
	t->sym_table_ptr = NULL;
	t->children = NULL;
	t->induced_type_by_coercion = IKS_INVALID;
	t->next = NULL;
	t->expectedTypes = NULL;
	t->context = current_context;

	if (type == AST_PROGRAMA) {
		/* 'program' has 1 child: the first function. 
		 * the subsequent functions will be nexts of this function. 
		 * if that child is NULL, it is an empty program. */
		ast_create_children(t, 1, args);
	} else if (type == AST_FUNCAO) {
		/* the first argument of an AST_FUNCAO node is the pointer to the 
		 * symbols table, which will contain the name of the function being 
		 * declared.*/ 
		t->sym_table_ptr 
		 	= (comp_dict_item_t*) va_arg(args, comp_dict_item_t*);		
		/* function has 1 child: the first command of the block of commands
		 * that it contains. the subsequent commands to that will be 'nexts'
		 * of that function. */
		ast_create_children(t, 1, args);
	} else if (type == AST_IF_ELSE) {
		/* if-else has 3 children: 1. the test, 2. the true case, 3. the false
		 * case. if there is no 'else', the false case must be passed as NULL,
		 * otherwise it will give undefined behaviour. */
		ast_create_children(t, 3, args);
	} else if (type == AST_DO_WHILE) {
		/* do-while has 2 children: the test, and the block of commands that
		 * it contains. */
		ast_create_children(t, 2, args);
	} else if (type == AST_WHILE_DO) {
		/* while-do has 2 children: the test, and the block of commands that
		* it contains. */
		ast_create_children(t, 2, args);
	} else if (type == AST_INPUT) {
		/* input has 1 child: the identifier that the input is directed to. */
		ast_create_children(t, 2, args);
	} else if (type == AST_OUTPUT) {
		/* output has 1 child: the expression list that is being output. 
		 * each element of the list will be 'next' of the previous element. */
		ast_create_children(t, 1, args);
	} else if (type == AST_ATRIBUICAO) {
		/* atribution has 2 children: the identifier that is being atributed to
		 * and the expression that is being atributed. */
		ast_create_children(t, 2, args);

		if (t->children[0]->type == AST_IDENTIFICADOR) {
			if (t->children[1]->type == AST_LITERAL &&
				get_type(t->children[1], PURPOSE_NORMAL) == IKS_STRING) {
				comp_context_symbol_t* sym = 
					context_find_identifier_multilevel(
						current_context, t->children[0]->sym_table_ptr->token);
				if (sym != NULL) {  /* sempre deveria ser != NULL*/  
					sym->data_size = max(
						strlen(t->children[1]->sym_table_ptr->value), 
						sym->data_size);
				}				
			}
		} else if (t->children[0]->type == AST_VETOR_INDEXADO) {
			if (t->children[0]->children[0]->type == AST_IDENTIFICADOR) {
				if (t->children[1]->type == AST_LITERAL &&
					get_type(t->children[1], PURPOSE_NORMAL) == IKS_STRING) {
					comp_context_symbol_t* sym =
						context_find_identifier_multilevel(
							current_context, 
							t->children[0]->children[0]->sym_table_ptr->token);
					if (sym != NULL) {  /* sempre deveria ser != NULL*/
						sym->data_size = max(
							sym->vector_size * strlen(
								t->children[1]->sym_table_ptr->value),
							sym->data_size);
					}
				}
			}
		}
	} else if (type == AST_RETURN) {
		/* return has 1 child: the expression that is being returned. */
		ast_create_children(t, 1, args);
	} else if (type == AST_BLOCO) {
		/* a block has 1 child: the first statement in the block that is being
		 * created.  */
		ast_create_children(t, 1, args);
	} else if (type == AST_IDENTIFICADOR || type == AST_LITERAL) {
		/* these contain 0 children. the first and only argument is the 
		 * pointer to the symbols table. */
		t->sym_table_ptr 
			= (comp_dict_item_t*) va_arg(args, comp_dict_item_t*);

	} else if (type == AST_ARIM_DIVISAO || type == AST_ARIM_MULTIPLICACAO
		|| type == AST_ARIM_SOMA || type == AST_ARIM_SUBTRACAO 
		|| type == AST_LOGICO_COMP_DIF || type == AST_LOGICO_E
		|| type == AST_LOGICO_OU || type == AST_LOGICO_COMP_IGUAL
		|| type == AST_LOGICO_COMP_LE || type == AST_LOGICO_COMP_GE
		|| type == AST_LOGICO_COMP_L || type == AST_LOGICO_COMP_G) {
		/* arithmetic and logical operations with two operands have two 
		 * children: the two operands. */
		ast_create_children(t, 2, args);
	} else if (type == AST_ARIM_INVERSAO || type == AST_LOGICO_COMP_NEGACAO) {
		/* arithmetic and logicaloperations with one operands have one child: 
		 * the operand */
		ast_create_children(t, 1, args);
	} else if (type == AST_VETOR_INDEXADO) {
		/* indexed vectors have two children: the vector's identifier and the
		 * value of the index. */
		ast_create_children(t, 2, args);
	} else if (type == AST_CHAMADA_DE_FUNCAO) {
		/* function calls have two children: the function name identifier
		 * and the first element of the expression list that corresponds 
		 * to its arguments. */
		ast_create_children(t, 2, args);
	}


	if (type == AST_PROGRAMA) {
		/* if the node is the root of the tree, set the global tree as the 
		 * node that we just created. */
		global_syntax_tree = t;
	}

	return t;
}

comp_tree_t* ast_list(comp_tree_t* first, comp_tree_t* next) {
	if (first == NULL && next != NULL)
		return next;	
	if (first != NULL && next != NULL)
		first->next = next;
	return first;
}

void ast_create_children(comp_tree_t* t, int num_children, va_list args) 
{
	t->num_children = num_children;
	t->children = (comp_tree_t**)
		malloc(t->num_children * sizeof(comp_tree_t*));
	int i;
	for (i = 0; i < num_children; ++i)
		t->children[i] = va_arg(args, comp_tree_t*);
}

void ast_generate_dot_graph(comp_tree_t* t) {
#ifdef AVALIACAO_ETAPA_3
	if (t == NULL)
		return;

	if (t->type == AST_LITERAL || t->type == AST_FUNCAO 
		|| t->type == AST_IDENTIFICADOR) {
		if (t->sym_table_ptr == NULL) {
			/* There was an error in the creation function. */
			printf("Error: sym table ptr for tree node is NULL, where it"
				"shouldn't be.");
			abort();
		}
		gv_declare(t->type, t, (void*)t->sym_table_ptr->token);
	} else { /* anything else that doesn't have a name */
		gv_declare(t->type, t, NULL);
	}

	int i;
	for (i = 0; i < t->num_children; ++i) {
		if (t->children[i] != NULL) {
			ast_generate_dot_graph(t->children[i]);
			gv_connect(t, t->children[i]);
		}		
	}

	if (t->next != NULL) {
		ast_generate_dot_graph(t->next);
		gv_connect(t, t->next);
	}
#endif
}

int ast_retrieve_node_purpose(comp_tree_t* node) {
	if (node->type == AST_IDENTIFICADOR)
		return PURPOSE_NORMAL;
	else if (node->type == AST_VETOR_INDEXADO) {
		yyerror("Indexed vector found");
		return PURPOSE_VECTOR;
	} else if (node->type == AST_CHAMADA_DE_FUNCAO)
		return PURPOSE_FUNCTION;
	else
		return 3;
}

