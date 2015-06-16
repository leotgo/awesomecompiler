#ifndef CC_BB_GRAPH_H
#define CC_BB_GRAPH_H

typedef struct bb_node_t {
	int first_instr_index;
	int last_instr_index;
	struct bb_node_t** next;
	struct bb_node_t** previous;
	int num_next;
	int num_previous;
	int visited;
} bb_node_t;

typedef struct bb_graph_t {
	bb_node_t** nodes;
	int num_nodes;
} bb_graph_t;

typedef struct dom_tree_t {
	bb_node_t* block;
	struct dom_tree_t** children;
	int num_children;
} dom_tree_t;

void free_bb_graph(bb_graph_t* g);

#endif 
