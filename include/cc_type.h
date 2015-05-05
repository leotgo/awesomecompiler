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

// Type list structure for AST nodes, used to determine whether the
// node's children agree to expected types.
typedef struct type_list {
	int type;
	struct type_list* next;
} type_list;

// Adds a type to the specified type list
type_list* type_list_Add(type_list* list, int addedType);

// Compares two type lists and returns true if they are 
// identical - same elements in same order
int type_list_Compare(type_list* list_a, type_list* list_b);

void type_list_free(type_list* x);

int coercion_possible(int type, int expected_type);

struct comp_context_symbol_t* get_symbol(struct comp_tree_t* node);

int type_check(struct comp_tree_t* ast);

int type_check_function(struct comp_tree_t* node);

int type_check_if_else(struct comp_tree_t* node);

int type_check_do_while(struct comp_tree_t* node);

int type_check_while_do(struct comp_tree_t* node);

int type_check_input(struct comp_tree_t* node);

int type_check_output(struct comp_tree_t* node);

int type_check_output_exp_list(struct comp_tree_t* node);

int type_check_attribution(struct comp_tree_t* node);

int type_check_indexed_vector(struct comp_tree_t* node);

int type_check_function_call(struct comp_tree_t* node);

int type_inference(struct comp_tree_t* node);

int type_binOperation_result(int childType_1, int childType_2);

int typeConvert(int type);

int get_type(struct comp_tree_t* node, int purpose);

struct comp_context_symbol_t* get_symbol(struct comp_tree_t* node);

int check_purpose(int purpose, struct comp_context_symbol_t* node_symbol);

int check_function(struct comp_tree_t* node, struct comp_tree_t* arguments);

int check_types(int variable_type, int expression_type);

int coercion_possible(int type, int expected_type);

#endif
