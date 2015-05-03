
#include "cc_type.h"
#include "cc_tree.h"
#include "cc_context.h"
#include "cc_error.h"
#include "../parser.h"
#include "main.h"

#define bool int
#define true 1
#define false 0

// Adds a type to the specified type list, at the end
// If list is null, memory is allocated for the list
type_list* type_list_Add(type_list* list, int addedType)
{
	if(list != NULL)
	{
		type_list* last = list;
		while(last->next != NULL) 
			last = last->next;

		last->next = (type_list*)malloc(sizeof(type_list));
		last->next->type = addedType;
		last->next->next = NULL;
	}
	else
	{
		list = (type_list*)malloc(sizeof(type_list));
		list->type = addedType;
		list->next = NULL;
	}
	
	return list;
}

// Compares two type lists and returns true if they are identical
// (same elements in same order)
int type_list_Compare(type_list* list_a, type_list* list_b)
{
	// Compare each element
	while(list_a != NULL && list_b != NULL)
	{
		if(list_a->type != list_b->type)
			return 0;
		else
		{
			list_a = list_a->next;
			list_b = list_b->next;
		}
	}
	/* In case there are no elements left in any of the lists, 	 |
	| we begin to check whether they have same size 		*/
	if(list_a == NULL && list_b == NULL) 
		return 1;
	else if(list_a == NULL && list_b != NULL)
		return 0;
	else if(list_a != NULL && list_b == NULL)
		return 0;
	else
		return 1;
}

int type_check(comp_tree_t* ast)
{
	if	(ast->type == AST_PROGRAMA)
		type_check(ast->children[0]);
	else if	(ast->type == AST_FUNCAO)
		type_check_function(ast);
	else if	(ast->type == AST_IF_ELSE)
		type_check_if_else(ast);
	else if	(ast->type == AST_DO_WHILE)
		type_check_do_while(ast);
	else if	(ast->type == AST_WHILE_DO)
		type_check_while_do(ast);	
	else if	(ast->type == AST_INPUT)
		type_check_input(ast);
	else if	(ast->type == AST_OUTPUT)
		type_check_output(ast);
	else if	(ast->type == AST_ATRIBUICAO)
		type_check_attribution(ast);

	if(ast->next != NULL)
		return type_check(ast->next);
	else
		return 1;
}

int type_check_function(comp_tree_t* node)
{
	comp_tree_t* ret = node->children[0];
	
	if( ret == NULL )
		return 0;

	// Search for return node, to check whether the type matches
	// the function type.
	while(ret->type != AST_RETURN && ret->next != NULL)
		ret = ret->next;
	
	if(ret->type != AST_RETURN)
	{
		yyerror("ERROR: Function has no RETURN statement defined");
		exit(IKS_ERROR_FUNCTION_NO_RETURN);
	}
	else if(ret->next)
	{
		yyerror("WARNING: Code after RETURN statement will never be executed");
	}

	int return_type = typeConvert( typeConvert( get_type ( ret->children[0] ) ) );
	
	comp_context_symbol_t* node_symbol;
	node_symbol = context_find_identifier_multilevel(current_context, node->sym_table_ptr->token);
	int function_type = typeConvert( node_symbol->type );

	if(return_type != function_type)
	{
		yyerror("ERROR: Return type does not match function type");
		exit(IKS_ERROR_WRONG_TYPE);
		return 0;
	}
	else
	{
		int childCheck = type_check(node->children[0]);
		if(node->next != NULL)
			return type_check(node->next);
		else
			return childCheck;
	}
}

int type_check_if_else(comp_tree_t* node)
{
	// still to implement
	return 1;
}

int type_check_do_while(comp_tree_t* node)
{
	// still to implement
	return 1;
}

int type_check_while_do(comp_tree_t* node)
{
	// still to implement
	return 1;
}

int type_check_input(comp_tree_t* node)
{
	if(node->children[0]->type != AST_IDENTIFICADOR)
	{
		yyerror("ERROR: INPUT only supports identifiers as arguments");
		exit(IKS_ERROR_WRONG_TYPE);
		return 0;
	}
	else
	{
		// Call type_inference function to determine whether the identifier exists
		type_inference(node->children[0]);
		return 1;
	}
}

int type_check_output(comp_tree_t* node)
{
	if(node->children[0]->type == AST_ARIM_SOMA 		|| 
	   node->children[0]->type == AST_ARIM_SUBTRACAO	||
	   node->children[0]->type == AST_ARIM_MULTIPLICACAO 	|| 
	   node->children[0]->type == AST_ARIM_DIVISAO		||
	   node->children[0]->type == AST_ARIM_INVERSAO		||
	   node->children[0]->type == AST_LITERAL		 )
	{
		return type_check_output_exp_list(node->children[0]);
	}
	else
	{
		yyerror("ERROR: OUTPUT only supports string literals or arithmetic expressions as arguments");
		exit(IKS_ERROR_WRONG_TYPE);
		return 0;
	}
}

int type_check_output_exp_list(comp_tree_t* node)
{
	if(node->type == AST_ARIM_SOMA 		|| 
	   node->type == AST_ARIM_SUBTRACAO	||
	   node->type == AST_ARIM_MULTIPLICACAO 	|| 
	   node->type == AST_ARIM_DIVISAO		||
	   node->type == AST_ARIM_INVERSAO		 )
	{
	
		// Call type_inference to check whether the expression has forming errors
		type_inference(node);
	
		// Go for next expression in expression list
		if(node->next != NULL) {
			return type_check_output_exp_list(node->next);
		}
	}
	else if(node->type == AST_LITERAL) 
	{
		int literalType = typeConvert( get_type( node ) );
		if(literalType != IKS_STRING)
		{
			yyerror("ERROR: OUTPUT only supports string literals or arithmetic expressions as arguments");
			exit(IKS_ERROR_WRONG_TYPE);
			return 0;
		}

		if(node->next != NULL) {
			return type_check_output_exp_list(node->next);
		}
	}
	else 
	{
		yyerror("ERROR: One of the arguments of OUTPUT is not an expression or literal");
		exit(IKS_ERROR_WRONG_TYPE);
		return 0;
	}

	return 1;
}

int type_check_attribution(comp_tree_t* node)
{
	int var_type = typeConvert( get_type( node->children[0] ) );
	int exp_type = typeConvert( get_type( node->children[1] ) );

	if(var_type != exp_type)
	{
		yyerror("ERROR: Expression type does not match target variable type");
		exit(IKS_ERROR_WRONG_TYPE);
	}		

	return 1;
}

// type_inference function receives an expression (it has to be an expression! such as +, -, *, / or literals and identifiers
int type_inference(comp_tree_t* node)
{
	if(node->type == AST_LITERAL || node->type == AST_IDENTIFICADOR)
	{
		int type =  typeConvert( get_type(node) );
		return type;
	}
	else if(node->type == AST_ARIM_SOMA 		|| 
		node->type == AST_ARIM_SUBTRACAO 	||
		node->type == AST_ARIM_MULTIPLICACAO 	|| 
		node->type == AST_ARIM_DIVISAO		 )
	{
		// Get children nodes type
		int childType_1 = typeConvert( get_type(node->children[0]) );
		int childType_2 = typeConvert( get_type(node->children[1]) );	

		if(childType_1 == IKS_STRING || childType_1 == IKS_CHAR || childType_2 == IKS_STRING || childType_2 == IKS_CHAR)
		{
			yyerror("ERROR: STRING and CHAR type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_WRONG_TYPE);
		}

		return type_binOperation_result(childType_1, childType_2);
		
	}
	else if(node->type == AST_ARIM_INVERSAO)
	{
		int childType = typeConvert( get_type(node->children[0]) );
		if(childType == IKS_STRING || childType == IKS_CHAR)
		{
			yyerror("ERROR: STRING and CHAR type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_WRONG_TYPE);
		}

		return childType;
	}
	else if(node->type == AST_LOGICO_E 			|| 
		node->type == AST_LOGICO_OU 			||
		node->type == AST_LOGICO_COMP_DIF 		|| 
		node->type == AST_LOGICO_COMP_LE 		||
		node->type == AST_LOGICO_COMP_GE		||	 
		node->type == AST_LOGICO_COMP_L 		||
		node->type == AST_LOGICO_COMP_G 		 )
		
	{
		int childType_1 = typeConvert( get_type(node->children[0]) );
		int childType_2 = typeConvert( get_type(node->children[1]) );
		
		if(childType_1 == IKS_STRING || childType_1 == IKS_CHAR || childType_2 == IKS_STRING || childType_2 == IKS_CHAR)
		{
			yyerror("ERROR: STRING and CHAR type terms are not supported in logical expression");
			exit(IKS_ERROR_WRONG_TYPE);
		}

		return IKS_BOOL;
	}
	else if(node->type == AST_LOGICO_COMP_NEGACAO)
	{
		int childType = typeConvert( get_type(node->children[0]) );
		if(childType != IKS_BOOL)
		{
			yyerror("ERROR: STRING and CHAR type terms are not supported in logical expression");
			exit(IKS_ERROR_WRONG_TYPE);
		}
		return childType;
	}
	else return IKS_INVALID;
}

int type_binOperation_result(int childType_1, int childType_2)
{
	if(childType_1 == childType_2)					// If they are the same type, return any of them
	{								// If not, use rules as defined in the task specification
		if(childType_1 == IKS_INT)
			return IKS_INT; 		
		else if(childType_1 == IKS_FLOAT)
			return IKS_FLOAT;
		else if(childType_1 == IKS_BOOL)
			return IKS_BOOL;
		else
			return IKS_INVALID;	
	}							
	else if( ( (childType_1 == IKS_INT) && (childType_2 == IKS_FLOAT) ) || ( (childType_1 == IKS_FLOAT) && (childType_2 == IKS_INT) ) )
		return IKS_FLOAT;
	else if( ( (childType_1 == IKS_BOOL) && (childType_2 == IKS_INT) ) || ( (childType_1 == IKS_INT) && (childType_2 == IKS_BOOL) ) )
		return IKS_INT;
	else if( ( (childType_1 == IKS_BOOL) && (childType_2 == IKS_FLOAT) ) || ( (childType_1 == IKS_FLOAT) && (childType_2 == IKS_BOOL) ) )
		return IKS_FLOAT;
	else
		return IKS_INVALID;
}

int typeConvert(int type)
{
	if(type == IKS_INVALID || type == IKS_INT || type == IKS_FLOAT || type == IKS_BOOL || type == IKS_CHAR || type == IKS_STRING)
		return type;
	else
	{
		if	(type == SIMBOLO_LITERAL_INT)
			return IKS_INT;
		else if	(type == SIMBOLO_LITERAL_FLOAT)
			return IKS_FLOAT;
		else if	(type == SIMBOLO_LITERAL_BOOL)
			return IKS_BOOL;
		else if (type == SIMBOLO_LITERAL_CHAR)
			return IKS_CHAR;
		else if (type == SIMBOLO_LITERAL_STRING)
			return IKS_STRING;
		else if	(type == TK_PR_INT)
			return IKS_INT;
		else if	(type == TK_PR_FLOAT)
			return IKS_FLOAT;
		else if	(type == TK_PR_BOOL)
			return IKS_BOOL;
		else if (type == TK_PR_CHAR)
			return IKS_CHAR;
		else if (type == TK_PR_STRING)
			return IKS_STRING;
		else {
			return IKS_INVALID;
		}
	}
}

int get_type(comp_tree_t* node/*, comp_tree_t* expression*/)
{	
	//printf("Entered get_type function\n\n");
	if(node->type == AST_IDENTIFICADOR)
	{

		comp_context_symbol_t* node_symbol;
		node_symbol = context_find_identifier_multilevel(current_context, node->sym_table_ptr->token);
		//node_symbol = context_find_identifier(current_context, node->sym_table_ptr->key);
	
		if(node_symbol == NULL)
		{	
			/* Literal not found in current context or in any of its parents */
			yyerror("Error: Undeclared variable");
			exit(IKS_ERROR_UNDECLARED);
		}
		else
		{
			/*//printf("%d \n\n", expression->sym_table_ptr->token_type);
			printf("teste %d, exp_type: %d\n", type_inference(expression), expression->num_children );
			getchar();
			if(check_types(node_symbol->type, type_inference(expression) ) == -1)
			{
				yyerror("Error: Wrong type");
				
				if(expression->sym_table_ptr->token_type == SIMBOLO_LITERAL_CHAR)
				{
					exit(IKS_ERROR_CHAR_TO_X);
				}
				else
				{
					if(expression->sym_table_ptr->token_type == SIMBOLO_LITERAL_STRING)
					{
						exit(IKS_ERROR_STRING_TO_X);
						
					}
					else
					{
						exit(IKS_ERROR_WRONG_TYPE);
					}
				}
			} */
			return typeConvert(node_symbol->type);
		}
	}
	else if(node->type == AST_LITERAL)
	{
		comp_dict_item_t* node_symbol;
		node_symbol = symbols_table_find(node->sym_table_ptr->key, &global_symbols_table);
		
		if(node_symbol == NULL)
		{
			/* Literal not found in current context (why would this happen?) */
			exit(IKS_ERROR_UNDECLARED);
		} 
		else
		{
			return typeConvert(node_symbol->token_type);
		}
	}
	else if(node->type == AST_ARIM_SOMA 		|| 
		node->type == AST_ARIM_SUBTRACAO 	||
		node->type == AST_ARIM_MULTIPLICACAO 	|| 
		node->type == AST_ARIM_DIVISAO 		||
		node->type == AST_ARIM_INVERSAO		 )
	{
		printf("Type inference for expression: %d \n", type_inference(node));
		return type_inference(node);
	}
	else if(node->type == AST_LOGICO_E 			|| 
		node->type == AST_LOGICO_OU 			||
		node->type == AST_LOGICO_COMP_DIF 		|| 
		node->type == AST_LOGICO_COMP_LE 		||
		node->type == AST_LOGICO_COMP_GE		||
		node->type == AST_LOGICO_COMP_L 		||
		node->type == AST_LOGICO_COMP_G 		||
		node->type == AST_LOGICO_COMP_NEGACAO 		 )
	{
		return type_inference(node);
	}
	else if(node->type == AST_CHAMADA_DE_FUNCAO)
	{
		comp_context_symbol_t* node_symbol;
		node_symbol = context_find_identifier_multilevel(current_context, node->children[0]->sym_table_ptr->token);
		return typeConvert( node_symbol->type );
	}
	else return node->type;
}

int check_function(comp_tree_t* node, comp_tree_t* arguments)
{
	/*printf("func: %s\n", node->sym_table_ptr->token);*/
	if(node->type == AST_IDENTIFICADOR)
	{
		comp_context_symbol_t* node_symbol;
		node_symbol = context_find_identifier_multilevel(current_context, node->sym_table_ptr->token);
	
		if(node_symbol == NULL)
		{
			/* function not found in current context or in any of its parents */
			yyerror("Error: Undeclared function");
			exit(IKS_ERROR_UNDECLARED);
		}
		else
		{
			if(node_symbol->purpose != FUNCTION)
			{
				if(node_symbol->purpose == NORMAL)
				{
					yyerror("Error: Trying to use variable as function");
					exit(IKS_ERROR_VARIABLE);
				}
				else
				{
					if(node_symbol->purpose == VECTOR)
					{
						yyerror("Error: Trying to use vector as function");
						exit(IKS_ERROR_VECTOR);		
					}	
				}
			}/*
			printf("verificando chamada de funcao");
			getchar();
			if(arguments->expectedTypes != NULL)
			{
				printf("arguments: %d", arguments->expectedTypes[0].type);
			}
			else
			{
				printf("arguments types null");
			}*/
		}
	}
}

int check_types(int variable_type, int expression_type)
{
	if( variable_type == TK_PR_INT && expression_type == SIMBOLO_LITERAL_INT)
	{
		return TK_PR_INT;
	}

	if( variable_type == TK_PR_CHAR && expression_type == SIMBOLO_LITERAL_CHAR)
	{
		return TK_PR_CHAR;
	}

	if( variable_type == TK_PR_FLOAT && expression_type == SIMBOLO_LITERAL_FLOAT)
	{
		return TK_PR_FLOAT;
	}

	if( variable_type == TK_PR_BOOL && expression_type == SIMBOLO_LITERAL_BOOL)
	{
		return TK_PR_BOOL;
	}

	if( variable_type == TK_PR_STRING && expression_type == SIMBOLO_LITERAL_STRING)
	{
		return TK_PR_STRING;
	}
	
	// conversao implicita de int pra float
	if( variable_type == TK_PR_FLOAT && expression_type == SIMBOLO_LITERAL_INT)
	{
		return TK_PR_FLOAT;
	}

	// conversao implicita de int pra bool
	if( variable_type == TK_PR_BOOL && expression_type == SIMBOLO_LITERAL_INT)
	{
		return TK_PR_BOOL;
	}

	// conversao implicita de bool pra int
	if( variable_type == TK_PR_INT && expression_type == SIMBOLO_LITERAL_BOOL)
	{
		return TK_PR_INT;
	}

	// conversao implicita de bool pra float
	if( variable_type == TK_PR_FLOAT && expression_type == SIMBOLO_LITERAL_BOOL)
	{
		return TK_PR_FLOAT;
	}

	// conversao implicita de float pra int
	if( variable_type == TK_PR_INT && expression_type == SIMBOLO_LITERAL_FLOAT)
	{
		return TK_PR_INT;
	}

	// conversao implicita de float pra bool
	if( variable_type == TK_PR_BOOL && expression_type == SIMBOLO_LITERAL_FLOAT)
	{
		return TK_PR_BOOL;
	}

	return -1;
}

