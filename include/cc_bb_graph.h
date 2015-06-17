#ifndef CC_BB_GRAPH_H
#define CC_BB_GRAPH_H

typedef struct bb_node_t {
	int first_instr_index;
	int last_instr_index;
	struct bb_node_t** next;
	struct bb_node_t** previous;
	int num_next;
	int num_previous;
} bb_node_t;

typedef struct bb_node_list_t {
	bb_node_t* node;
	struct bb_node_list_t* next;
} bb_node_list_t;

typedef struct bb_graph_t {
	bb_node_t** nodes;
	int num_nodes;
} bb_graph_t;

typedef struct bb_loop_t {
	bb_node_t* start_block;
	bb_node_t* jump_block;
	bb_node_t* exit_block;
} bb_loop_t;

typedef struct dom_tree_t {
	bb_node_t* block;
	struct dom_tree_t* parent;
	struct dom_tree_t** children;
	int num_children;
} dom_tree_t;

void free_bb_graph(bb_graph_t* g);

#endif 
