#include "cc_tree.h"

comp_tree_t* global_syntax_tree = NULL;

void free_tree_node(comp_tree_t* t) {
	if (t == NULL) return;
	if (t->children != NULL && t->num_children > 0) {
		for (int i = 0; i < t->num_children; ++i)
			free_tree_node(t->children[i]);
	}
	if (t->next) free_tree_node(t->next);
	if (t->children)
		free(t->children);
	free(t);
}

comp_tree_t* ast_create(int type, ...) {
	va_list args;
	va_start(args, type);
	comp_tree_t* ans = ast_createv(type, args);
	va_end(args);
	return ans;	
}

comp_tree_t* ast_createv(int type, va_list args) {
	comp_tree_t* t = (comp_tree_t*) malloc(sizeof(comp_tree_t));
	t->type = type;
	t->num_children = 0;
	t->sym_table_ptr = NULL;
	t->children = NULL;
	t->next = NULL;

	if (type == AST_PROGRAMA) {
		/* 'program' has 1 child: the first function. 
		 * the subsequent functions will be nexts of this function. 
		 * if that child is NULL, it is an empty program. */
		ast_create_children(t, 1, args);
	} else if (type == AST_FUNCAO) {
		/* function has 1 child: the first command of the block of commands
		 * that it contains. the subsequent commands to that will be 'nexts'
		 * of that function. */
		ast_create_children(t, 1, args);

		/* IMPORTANT: we have to add somehow the function identifier here, so
		 * that we can show the functions name in the .dot format. maybe store 
		 * a pointer to the symbols table? */
		
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
		ast_create_children(t, 1, args);
	} else if (type == AST_OUTPUT) {
		/* output has 1 child: the expression list that is being output. 
		 * each element of the list will be 'next' of the previous element. */
		ast_create_children(t, 1, args);
	} else if (type == AST_ATRIBUICAO) {
		/* atribution has 2 children: the identifier that is being atributed to
		 * and the expression that is being atributed. */
		ast_create_children(t, 2, args);
	} else if (type == AST_RETURN) {
		/* return has 1 child: the expression that is being returned. */
		ast_create_children(t, 1, args);
	} else if (type == AST_BLOCO) {
		/* block isn't being used right now. the reason for this is that
		 * this node will not be shown in the ast graphical representation, and 
		 * we're already representing a command block (or list) through the
		 * 'next' field. */
	} else if (type == AST_IDENTIFICADOR || type == AST_LITERAL) {
		/* */



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

void ast_create_children(comp_tree_t* t, int num_children, va_list args) {
	t->num_children = num_children;
	t->children = (comp_tree_t**)
		malloc(t->num_children * sizeof(comp_tree_t*));
	for (int i = 0; i < num_children; ++i)
		t->children[i] = va_arg(args, comp_tree_t*);
}

void ast_generate_dot_graph(comp_tree_t* t) {
	/* todo: read the documentation on the dot format and the
	 * given gv_declare and gv_connect.
	 * 
	 * implement this. */
}
