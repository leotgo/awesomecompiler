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



#endif

