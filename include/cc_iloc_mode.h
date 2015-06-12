#ifndef CC_ILOC_MODE_H
#define CC_ILOC_MODE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cc_iloc.h"
#include "cc_bb_graph.h"

int iloc_mode(FILE* f);

/* 
 * takes the iloc_mode_instr_list and generates the graph of basic blocks for
 * it. the result is stured in the global bb_graph variable;
 * 
 * */
void generate_bb_graph();

/* puts all the instructions in iloc_mode_instr_list in an array, so we can
 * use random acess iterators.*/
void generate_instr_array();

extern instruction* instr_array;
extern instruction* iloc_mode_instr_list;
extern bb_graph_t* bb_graph;

#endif