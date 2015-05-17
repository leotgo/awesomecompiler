#ifndef __TYPE_H
#define __TYPE_H

#include "cc_tree.h"
#include "cc_context.h"
#include "cc_dict.h"
#include "cc_error.h"
#include "../parser.h"
#include "main.h"



#define IKS_INVALID 0
#define IKS_INT 1
#define IKS_FLOAT 2
#define IKS_CHAR 3
#define IKS_STRING 4
#define IKS_BOOL 5

#define ERROR 0
#define SUCCESS 1
 
// Type list structure for AST nodes, used to determine whether the
// node's children agree to expected types.
typedef struct type_list 
{
	int type;
	struct type_list* next;
} type_list;

// Adds a type to the specified type list
type_list* type_list_Add(type_list* list, int addedType);

// free all nodes from a type_list
void type_list_free(type_list* x);


/*
	checks type depending on the lexic symbol
*/
int type_check(struct comp_tree_t* ast);

/*
	checks if return matches return type from function 
*/
int type_check_function_definition(struct comp_tree_t* node);

/*
	checks if type from if is boolean and checks type of their command blocks
*/
int type_check_if_else(struct comp_tree_t* node);

/*
	checks if while statement type is bool and type checks the command block
*/
int type_check_do_while(struct comp_tree_t* node);

/*
	checks if while do statement type is bool and type checks the command block
*/
int type_check_while_do(struct comp_tree_t* node);

/*
	checks if input statement type is an identifier
*/
int type_check_input(struct comp_tree_t* node);

/*
	checks if output statement type is string literal ou arithmetic expression
*/
int type_check_output(struct comp_tree_t* node);

/*
	checks if output list statement type is string literal ou arithmetic expression
*/
int type_check_output_exp_list(struct comp_tree_t* node);

/*
	checks if atttribution has the right type or type can be coerced
*/
int type_check_attribution(struct comp_tree_t* node);

/*
	checks type from indexed vector, also seeing if variable was declared as one
*/
int type_check_indexed_vector(struct comp_tree_t* node);

int type_check_vector_dimension(struct comp_tree_t* node);

int check_vector_dimensions_number(struct comp_tree_t* node);
/*
	inferences type given an expression
	type_inference function receives an expression (it has to be an expression! such as +, -, *, / or literals and identifiers
	returns infered type
*/
int type_inference(struct comp_tree_t* node);

/*
	returns type from operation 
*/
int operation_type(int childType_1, int childType_2);

/*
	converts from all types of different definitions of types to a unified enumeration
	returns type in unified enumeration
*/
int typeConvert(int type);

/*
	gets type from ast node. If node represents a function, variable or vector
	the second parameter should be its purpose (enum that indicates if is used as
	function, variable or vector)
*/
int get_type(struct comp_tree_t* node, int purpose);

/*
	gets symbol from symbol's table from the ast node 
*/
struct comp_context_symbol_t* get_symbol(struct comp_tree_t* node);

/*
	checks if variable purpose is the same as defined
*/
int check_purpose(int purpose, struct comp_context_symbol_t* node_symbol);

/*
	checks if function calls have the right parameter types 
	and if identifier was really defined as a function
	node: identifier for the function name
	arguments: ast item with list that contains the function arguments
*/
int type_check_function_call(struct comp_tree_t* node, struct comp_tree_t* arguments);

/*
	tests if type coercion is possible
	returns boolean to wether coercion is possible or not
*/
int coercion_possible(int type, int expected_type);

#endif
