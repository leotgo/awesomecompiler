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

void free_dom_tree();
void generate_dom_tree();
int is_dominated_by(bb_node_t* current, bb_node_t* target, bb_node_t* start, bb_node_list_t* visited);
void print_dom_tree(dom_tree_t* t, int level);
void print_graph();

void loop_optimization();
void optimize_loop(bb_loop_t* loop);
void print_loop(bb_loop_t* loop);
int check_node_domination(bb_node_t* dominator, bb_node_t* dominated);
int check_node_in_tree(dom_tree_t* root, bb_node_t* node);

int node_list_contains(bb_node_list_t* list, bb_node_t* node);

#endif
