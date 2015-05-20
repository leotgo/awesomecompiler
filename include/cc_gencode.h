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
void generate_code(comp_tree_t* node, char* rdest);

#endif

/* 
 * pergunta pro professor:
 * 
 * dá pra carregar uma string em um registrador iloc?
 * 
 * como o senhor recomenda carregar as strings?
 * 
 * */


/* 
 * TODO alex:
 * 
 * Mudar tamanho das strings para 100
 * Tamanho dos vetores multidimensionais
 * Enderecamento dos vetores
 * Implementar AST_ in generate_code
 * 
 * */