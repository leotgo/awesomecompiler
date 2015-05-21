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

void generate_code_if_else(comp_tree_t* node);

void generate_code_do_while(comp_tree_t* node);

void generate_code_while_do(comp_tree_t* node);

#endif

