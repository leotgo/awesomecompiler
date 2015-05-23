
#include "cc_type.h"

#define bool int
#define true 1
#define false 0


// Adds a type to the specified type list, at the beginning
type_list* type_list_Add(type_list* list, int addedType)
{
		type_list* new = (type_list*)malloc(sizeof(type_list));
		new->type = addedType;
		new->next = list;
		return new;
}

// free all nodes from a type_list
void type_list_free(type_list* x) 
{
	while (x != NULL) 
	{
		type_list* y = x;
		x = x->next;
		free(y);
	}
}

/*
	checks type depending on the lexic symbol
*/
int type_check(comp_tree_t* ast)
{
	
	if	(ast == NULL)
		return 1;
	if	(ast->type == AST_PROGRAMA)
		type_check(ast->children[0]);
	else if	(ast->type == AST_FUNCAO)
		type_check_function_definition(ast);
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
	else if (ast->type == AST_VETOR_INDEXADO)
		type_check_indexed_vector(ast);
	else if (ast->type == AST_CHAMADA_DE_FUNCAO)
		type_check_function_call(ast, ast->children[1]);
	
	if(ast->next != NULL)
		return type_check(ast->next);
	else
		return 1;
}

/*
	checks if return matches return type from function 
*/
int type_check_function_definition(comp_tree_t* node)
{
	comp_tree_t* ret = node->children[0];
	
	if( ret == NULL )
		return ERROR;

	// Search for return node, to check whether the type matches
	// the function type.
	while(ret->type != AST_RETURN && ret->next != NULL)
	{
		ret = ret->next;
	}
	
	if(ret->type == AST_RETURN)
	{
		// there is a return in the defined function
		
		if(ret->next)
		{
			// there is something after the 'return' command on function
			yyerror("WARNING: Code after RETURN statement will never be executed");
		}


		int return_type = type_convert(  get_type ( ret->children[0], ast_retrieve_node_purpose(ret->children[0]) ) ) ;

	
		comp_context_symbol_t* node_symbol;
		node_symbol = context_find_identifier_multilevel(node->context, node->sym_table_ptr->token);
		int function_type = type_convert( node_symbol->type );

		// tries coercion for return type and function return type
		if(!coercion_possible(return_type, function_type))
		{
			yyerror("ERROR: Return type does not match function type");
			exit(IKS_ERROR_WRONG_PAR_RETURN);
			return ERROR;
		}
		else
		{
			ret->induced_type_by_coercion = function_type;
		}
	}
	return SUCCESS;	
}

/*
	checks if type from if is boolean and checks type of their command blocks
*/
int type_check_if_else(comp_tree_t* node)
{
	int test_type = type_convert( get_type( node->children[0], ast_retrieve_node_purpose( node->children[0] ) ) );

	// child 0: test - has to be booleean
	if(!coercion_possible(test_type, IKS_BOOL))
	{
		yyerror("ERROR: If-Else test is not a boolean");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}
	else
	{
		node->children[0]->induced_type_by_coercion = IKS_BOOL;
	}

	// child 1: true case
	type_check(node->children[1]);

	// child 2: false case
	if(node->children[2] != NULL)
	{
		type_check(node->children[2]);
	}

	return SUCCESS;
}

/*
	checks if while statement type is bool and type checks the command block
*/
int type_check_do_while(comp_tree_t* node)
{
	int test_type = type_convert( get_type( node->children[0], ast_retrieve_node_purpose( node->children[0] ) ) );

	// child 0: test - has to be booleean
	printf("Type1: %d %d\n",test_type,IKS_BOOL);
	if(!coercion_possible(test_type, IKS_BOOL))
	{
		yyerror("ERROR: If-Else test is not a boolean");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}
	else
	{
		node->children[0]->induced_type_by_coercion = IKS_BOOL;
	}

	// child 1: true case
	return type_check(node->children[1]);
}

/*
	checks if while do statement type is bool and type checks the command block
*/
int type_check_while_do(comp_tree_t* node)
{
	int test_type = type_convert( get_type( node->children[0], ast_retrieve_node_purpose( node->children[0] ) ) );

	// child 0: test - has to be booleean
	
		printf("Type2: %d %d\n",test_type,IKS_BOOL);
	if(!coercion_possible(test_type, IKS_BOOL))
	{
		yyerror("ERROR: If-Else test is not a boolean");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}
	else
	{
		node->children[0]->induced_type_by_coercion = IKS_BOOL;
	}

	// child 1: true case
	return type_check(node->children[1]);
}

/*
	checks if input statement type is an identifier
*/
int type_check_input(comp_tree_t* node)
{
	if(node->children[0]->type != AST_IDENTIFICADOR)
	{
		yyerror("ERROR: INPUT only supports identifiers as arguments");
		exit(IKS_ERROR_WRONG_PAR_INPUT);
		return ERROR;
	}
	else
	{
		// Call type_inference function to determine whether the identifier exists
		type_inference(node->children[0]);
		return SUCCESS;
	}
}

/*
	checks if output statement type is string literal ou arithmetic expression
*/
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
		exit(IKS_ERROR_WRONG_PAR_OUTPUT);
		return ERROR;
	}
}

/*
	checks if output list statement type is string literal ou arithmetic expression
*/
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
		if(node->next != NULL) 
		{
			return type_check_output_exp_list(node->next);
		}
	}
	else if(node->type == AST_LITERAL) 
	{
		comp_dict_item_t* symbol = node->sym_table_ptr;
		int literalType = type_convert( symbol->token_type );
		if(literalType != IKS_STRING)
		{
			yyerror("ERROR: OUTPUT only supports string literals or arithmetic expressions as arguments");
			exit(IKS_ERROR_WRONG_PAR_OUTPUT);
			return ERROR;
		}

		if(node->next != NULL) 
		{
			return type_check_output_exp_list(node->next);
		}
	}
	else 
	{
		yyerror("ERROR: One of the arguments of OUTPUT is not an expression or literal");
		exit(IKS_ERROR_WRONG_PAR_OUTPUT);
		return ERROR;
	}

	return SUCCESS;
}

/*
	checks if atttribution has the right type or type can be coerced
*/
int type_check_attribution(comp_tree_t* node)
{
	int var_type = type_convert( get_type( node->children[0], ast_retrieve_node_purpose(node->children[0]) ) );
	int exp_type = type_convert( get_type( node->children[1], ast_retrieve_node_purpose(node->children[1]) ) );
	if(var_type != IKS_STRING && exp_type == IKS_STRING)
	{
		yyerror("ERROR: Impossible coertion from STRING type to variable type");
		exit(IKS_ERROR_STRING_TO_X);
		return ERROR;
	}
	else if(var_type != IKS_CHAR && exp_type == IKS_CHAR)
	{
		yyerror("ERROR: Impossible coertion from CHAR type to variable type");
		exit(IKS_ERROR_CHAR_TO_X);
		return ERROR;
	}
	else if(!coercion_possible(var_type, exp_type))
	{
		yyerror("ERROR: Expression type does not match target variable type");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}		

	return SUCCESS;
}
/*
	checks type from indexed vector, also seeing if variable was declared as one
*/
int type_check_indexed_vector(comp_tree_t* node)
{
	
	int exp_type = type_convert( get_type( node->children[1], ast_retrieve_node_purpose(node->children[1]) ) );

	if(exp_type == IKS_STRING)
	{
		yyerror("ERROR: Impossible coertion from STRING type to integer in vector access");
		exit(IKS_ERROR_STRING_TO_X);
		return ERROR;
	}
	else if(exp_type == IKS_CHAR)
	{
		yyerror("ERROR: Impossible coertion from CHAR type to integer in vector access");
		exit(IKS_ERROR_CHAR_TO_X);
		return ERROR;
	}
	else if(exp_type != IKS_INT)
	{
		yyerror("ERROR: Vector access expression is not an integer");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}

	return SUCCESS;
}

int type_check_vector_dimension(struct comp_tree_t* node)
{
	int exp_type = type_convert( type_inference(node) );

	if(exp_type == IKS_STRING)
	{
		yyerror("ERROR: Impossible coertion from STRING type to integer in vector access");
		exit(IKS_ERROR_STRING_TO_X);
		return ERROR;
	}
	else if(exp_type == IKS_CHAR)
	{
		yyerror("ERROR: Impossible coertion from CHAR type to integer in vector access");
		exit(IKS_ERROR_CHAR_TO_X);
		return ERROR;
	}
	else if(exp_type != IKS_INT)
	{
		yyerror("ERROR: Vector access expression is not an integer");
		exit(IKS_ERROR_WRONG_TYPE);
		return ERROR;
	}

	
	return SUCCESS;
}

int check_vector_dimensions_number(struct comp_tree_t* node)
{
	
	comp_context_symbol_t* symbol = get_symbol(node);
	
	if( node->vector_dimensions > symbol->vector_dimensions)
	{
		yyerror("ERROR: Using vector with more dimensions than defined");
		exit(IKS_ERROR_STRING_TO_X); //TODO colocar constante de exit certa!
		return ERROR;
	}
	else
	{
		if(node->vector_dimensions < symbol->vector_dimensions)
		{
			yyerror("ERROR: Using vector with less dimensions than defined");
			exit(IKS_ERROR_STRING_TO_X); //TODO colocar constante de exit certa!
			return ERROR;
		}
		else
		{
			return SUCCESS;
		}
	}
}

/*
	inferences type given an expression
	type_inference function receives an expression (it has to be an expression! such as +, -, *, / or literals and identifiers
	returns infered type
*/
int type_inference(comp_tree_t* node)
{
	if(node->type == AST_IDENTIFICADOR)
	{
		// if node is an identifier, gets type from context's symbols table
		int type =  type_convert( get_type(node, ast_retrieve_node_purpose(node) ));
		return type;
	}
	else if(node->type == AST_LITERAL)
	{
		// if node is literal gets type by global symbols table
		comp_dict_item_t* symbol= node->sym_table_ptr;
		int type = type_convert( symbol->token_type );
		return type;
	}
	else if(node->type == AST_ARIM_SOMA 		|| 
		node->type == AST_ARIM_SUBTRACAO 	||
		node->type == AST_ARIM_MULTIPLICACAO 	|| 
		node->type == AST_ARIM_DIVISAO		 )
	{
		// Get children nodes type
		int childType_1 = type_convert( get_type(node->children[0], ast_retrieve_node_purpose(node->children[0]) ));
		int childType_2 = type_convert( get_type(node->children[1], ast_retrieve_node_purpose(node->children[1]) ));	

		if(childType_1 == IKS_STRING ||  childType_2 == IKS_STRING)
		{
			yyerror("ERROR: STRING type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_STRING_TO_X);
		}
		else if(childType_1 == IKS_CHAR || childType_2 == IKS_CHAR)
		{
			yyerror("ERROR: CHAR type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_CHAR_TO_X);
		}

		return operation_type(childType_1, childType_2);
		
	}
	else if(node->type == AST_ARIM_INVERSAO)
	{
		int childType = type_convert( get_type(node->children[0], ast_retrieve_node_purpose(node->children[0]) ));
		if(childType == IKS_STRING)
		{
			yyerror("ERROR: STRING type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_STRING_TO_X);
		}
		else if(childType == IKS_CHAR)
		{
			yyerror("ERROR: CHAR type terms are not supported in arithmetic expression");
			exit(IKS_ERROR_CHAR_TO_X);
		}

		return childType;
	}
	else if(node->type == AST_LOGICO_E 			|| 
		node->type == AST_LOGICO_OU 			||
		node->type == AST_LOGICO_COMP_IGUAL 		|| 
		node->type == AST_LOGICO_COMP_DIF 		|| 
		node->type == AST_LOGICO_COMP_LE 		||
		node->type == AST_LOGICO_COMP_GE		||	 
		node->type == AST_LOGICO_COMP_L 		||
		node->type == AST_LOGICO_COMP_G 		 )
		
	{
		int childType_1 = type_convert( get_type(node->children[0], ast_retrieve_node_purpose(node->children[0]) ));
		int childType_2 = type_convert( get_type(node->children[1], ast_retrieve_node_purpose(node->children[1]) ));
		
		if(childType_1 != childType_2)
		{
			if(!coercion_possible(childType_1, childType_2))
			{
				if(!coercion_possible(childType_1, childType_2))
				{
					yyerror("ERROR: Comparison elements have different types");
					exit(IKS_ERROR_WRONG_TYPE);
					return 0;
				}
				else
					node->children[1]->induced_type_by_coercion = childType_1;
			}
			else
				node->children[0]->induced_type_by_coercion = childType_2;
		}

		return IKS_BOOL;
	}
	else if(node->type == AST_LOGICO_COMP_NEGACAO)
	{
		int childType = type_convert( get_type(node->children[0], ast_retrieve_node_purpose(node->children[0]) ));
		if(childType != IKS_BOOL)
		{
			yyerror("ERROR: STRING and CHAR type terms are not supported in logical expression");
			exit(IKS_ERROR_WRONG_TYPE);
		}
		return childType;
	}
	else return IKS_INVALID;
}

/*
	returns type from operation 
*/
int operation_type(int childType_1, int childType_2)
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

/*
	converts from all types of different definitions of types to a unified enumeration
	returns type in unified enumeration
*/
int type_convert(int type)
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

/*
	gets type from ast node. If node represents a function, variable or vector
	the second parameter should be its purpose (enum that indicates if is used as
	function, variable or vector)
*/
int get_type(comp_tree_t* node, int purpose)
{	

	if(node->type == AST_VETOR_INDEXADO)
	{
		
		type_check(node);
		return get_type(node->children[0], PURPOSE_VECTOR);
	}
	if(node->type == AST_IDENTIFICADOR)
	{

		comp_context_symbol_t* node_symbol;
		node_symbol = get_symbol(node);
			
		check_purpose(purpose, node_symbol);
		
		if(purpose == PURPOSE_VECTOR)
		{
			check_vector_dimensions_number(node);
		}
		
		return type_convert(node_symbol->type);
		
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
			return type_convert(node_symbol->token_type);
		}
	}
	else if(node->type == AST_ARIM_SOMA 		|| 
		node->type == AST_ARIM_SUBTRACAO 	||
		node->type == AST_ARIM_MULTIPLICACAO 	|| 
		node->type == AST_ARIM_DIVISAO 		||
		node->type == AST_ARIM_INVERSAO		 )
	{
		return type_inference(node);
	}
	else if(node->type == AST_LOGICO_E 			|| 
		node->type == AST_LOGICO_OU 			||
		node->type == AST_LOGICO_COMP_IGUAL 		|| 
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
		node_symbol = context_find_identifier_multilevel(node->context, node->children[0]->sym_table_ptr->token);

		if(node_symbol == NULL)
		{	
			/* Function not found in current context or in any of its parents */
			yyerror("Error: Undeclared function");
			exit(IKS_ERROR_UNDECLARED);
		}

		return type_convert( node_symbol->type );
	}
	else return node->type;
}

/*
	gets symbol from symbol's table from the ast node 
*/
comp_context_symbol_t* get_symbol(const comp_tree_t* node)
{
	comp_context_symbol_t* node_symbol;
	node_symbol = context_find_identifier_multilevel(node->context, 
		node->sym_table_ptr->token);

	if(node_symbol == NULL)
	{	
		/* Identifier not found in current context or in any of its parents */
		yyerror("ERROR: Undeclared variable");
		exit(IKS_ERROR_UNDECLARED);
	}
	else
	{
		return node_symbol;
	}
}

/*
	checks if variable purpose is the same as defined
*/
int check_purpose(int purpose, comp_context_symbol_t* node_symbol)
{
	if(node_symbol->purpose == PURPOSE_VECTOR && purpose == PURPOSE_NORMAL)
	{
		yyerror("ERROR: Vector is being used as variable");
		exit(IKS_ERROR_VECTOR);
	}

	if(node_symbol->purpose == PURPOSE_NORMAL && purpose == PURPOSE_VECTOR)
	{
		yyerror("ERROR: Variable is being used as vector");
		exit(IKS_ERROR_VARIABLE);
	}

	if(node_symbol->purpose == PURPOSE_FUNCTION && purpose == PURPOSE_NORMAL)
	{
		yyerror("ERROR: Function is being used as variable");
		exit(IKS_ERROR_FUNCTION);
	}

	if(node_symbol->purpose == PURPOSE_FUNCTION && purpose == PURPOSE_VECTOR)
	{
		yyerror("ERROR: Function is being used as vector");
		exit(IKS_ERROR_FUNCTION);
	}

	if(node_symbol->purpose == PURPOSE_NORMAL && purpose == PURPOSE_FUNCTION)
	{
		yyerror("ERROR: Trying to use variable as function");
		exit(IKS_ERROR_VARIABLE);
	}

	if(node_symbol->purpose == PURPOSE_VECTOR && purpose == PURPOSE_FUNCTION)
	{
		yyerror("ERROR: Trying to use vector as function");
		exit(IKS_ERROR_VECTOR);		
	}	
	

	return SUCCESS;

}

/*
	checks if function calls have the right parameter types 
	and if identifier was really defined as a function
	node: identifier for the function name
	arguments: ast item with list that contains the function arguments
	
*/
int type_check_function_call(comp_tree_t* node, comp_tree_t* arguments)
{

	if(node->type == AST_IDENTIFICADOR)
	{
		comp_context_symbol_t* node_symbol;
		node_symbol = context_find_identifier_multilevel(node->context, node->sym_table_ptr->token);
		
		if(node_symbol == NULL)
		{
			/* function not found in current context or in any of its parents */
			
			yyerror("ERROR: Undeclared function");
			exit(IKS_ERROR_UNDECLARED);
			return ERROR;
		}
		else
		{
			
			if(node_symbol->purpose != PURPOSE_FUNCTION)
			{
				// the corresponding symbol from the symbol's table is 
				// not supposed to be a function
				
				if(node_symbol->purpose == PURPOSE_NORMAL)
				{
					yyerror("ERROR: Trying to use variable as function");
					exit(IKS_ERROR_VARIABLE);
					return ERROR;
				}
				else
				{
					if(node_symbol->purpose == PURPOSE_VECTOR)
					{
						yyerror("ERROR: Trying to use vector as function");
						exit(IKS_ERROR_VECTOR);		
						return ERROR;
					}	
				}
			}

			if((arguments != NULL)&&(node_symbol != NULL))
			{
				
				comp_tree_t* arg = arguments;
				type_list* expected_type = NULL;
				expected_type = node_symbol->parameters;
				comp_context_symbol_t* symbol;
				while (arg != NULL) 
				{
					// tests if all arguments from function call 
					// fits with parameters from fucntion definition
					if (expected_type == NULL) 
					{	
						// there are still arguments but no parameters
						// calls error message
						yyerror("ERROR: Excess arguments");
						exit(IKS_ERROR_EXCESS_ARGS);
						/* error: given more parameters than expected. */
						return ERROR;
					}

					// gets argument type
					int arg_type;
					if(arg->type != AST_LITERAL)
					{
						symbol = get_symbol(arg);
						arg_type = symbol->type;
					}
					else
					{
						arg_type = arg->sym_table_ptr->token_type;
					}
					
					// if coercion not possible, argument is of wrong type
					if (!coercion_possible(type_convert( arg_type ), type_convert(expected_type->type))) 
					{
						yyerror("ERROR: Arguments with wrong type");
						exit(IKS_ERROR_WRONG_TYPE_ARGS);
						/* error: parameter of invalid type, and no
						 * coercion is possible. */
						return ERROR;
					} 
					else 	
					{
						arg->induced_type_by_coercion = type_convert( expected_type->type );
					}
					expected_type = expected_type->next;
					arg = arg->next;
						
				}
				if (expected_type != NULL) 
				{
					// if there are still parameters but no more arguments, show error message
					yyerror("Error: Missing arguments");
					exit(IKS_ERROR_MISSING_ARGS);	
					/* error, given less parameters than expected. */
					return ERROR;
				}
			}
			else
			{
				
				if(node_symbol->parameters != NULL)
				{
					// function call has no arguments but function definition has
					yyerror("ERROR: Missing arguments");
					exit(IKS_ERROR_MISSING_ARGS);	
					return ERROR;
				}
			}
		}
	}
	return SUCCESS;
}


/*
	tests if type coercion is possible
	returns boolean to wether coercion is possible or not
*/
int coercion_possible(int type, int expected_type) 
{
	return type == expected_type || ( ((type == IKS_INT || type == IKS_FLOAT || type == IKS_BOOL) && (expected_type == IKS_INT || expected_type == IKS_FLOAT || expected_type == IKS_BOOL)));
}
