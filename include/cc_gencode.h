#ifndef CC_GENCODE_H
#define CC_GENCODE_H
#include "cc_tree.h"

/* this function takes a tree node and generates code for that node.
 * 
 * the function is required to call generate_code for all the children of that 
 * node.
 * 
 * if the operation has a result, it places the result in 'rdest'. otherwise,
 * it ignores 'rdest'.
 * 
 * */
void generate_code(comp_tree_t* node, char* regdest);

void generate_code_literal(comp_tree_t* node, char* regdest);

void generate_code_operation(comp_tree_t* node, char* regdest, int operation);

void generate_code_operation_negative(comp_tree_t* node, char* regdest);

void generate_children_code(comp_tree_t* node, char* regdest);

void generate_code_if_else(comp_tree_t* node);

void generate_code_do_while(comp_tree_t* node);

void generate_code_while_do(comp_tree_t* node);

void generate_code_vetor_indexado(comp_tree_t* node, char* regdest);

void generate_code_atribuicao(comp_tree_t* node, char* regdest);

void generate_code_identificador(comp_tree_t* node, char* regdest);

void generate_code_comparison(comp_tree_t* node, char* regdest, int comparison);
/* 
 * does the vector indexing for the given node (which MUST have type 
 * AST_VETOR_INDEXADO). it creates an instruction list and returns it. 
 * it DOES NOT change 'node', in any way.
 * 
 * the resulting address is stored in the register given by regdest_address. 
 * */
instruction* calculate_vector_indexing_address(const comp_tree_t* node, 
	char* regdest_address);


#endif

