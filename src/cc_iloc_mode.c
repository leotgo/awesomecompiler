#include "cc_iloc_mode.h"
#include <string.h>

instruction* iloc_mode_instr_list = NULL;
instruction* instr_array = NULL;
bb_graph_t* bb_graph = NULL;
dom_tree_t* dom_tree = NULL;
dom_tree_t** tree = NULL;

int num_instr = 0;

int compare_marked_instr(const void* a, const void* b) {
	if (*(int*)a < *(int*)b) return -1;
	if (*(int*)a == *(int*)b) return 0;
	return 1;
}


int iloc_mode(FILE* f) {
	int i;
	iloc_parse(f);

	generate_instr_array();

	generate_bb_graph();

	generate_dom_tree();

	for (i = 0; i < num_instr; ++i) {
		printf("%d  ", i + 1);
		print_instruction(&instr_array[i]);
	}

	printf("\n\nDominators tree:\n");
	print_dom_tree(dom_tree, 0);

	print_graph();
	loop_optimization();

	free_dom_tree();
	free_bb_graph(bb_graph);
	
	return 0;
}

void print_graph()
{
	printf("\n\nGraph:\n");
	int i;
	for(i = 0; i < bb_graph->num_nodes; i++)
	{
		printf("Node: ");
		print_instruction(&instr_array[bb_graph->nodes[i]->first_instr_index]);
		printf("\n");
	}
}

void print_dom_tree(dom_tree_t* t, int level)
{
	int l;
	for(l = 0; l < level; l++)
		printf("\t");

	print_instruction(&instr_array[ t->block->first_instr_index ]);
	printf("\n");

	int c;
	for(c = 0; c < t->num_children; c++)
		print_dom_tree(t->children[c], level + 1);
}

void free_dom_tree()
{
	int i;
	for(i = 0; i < bb_graph->num_nodes; i++)
		free(tree[i]->children);	

	free(tree);
	free(dom_tree);
}

void generate_instr_array() {
	/* first, add an extra instruction at the end, so the last instruction
	 * can be a leader instruction for a basic block, independently whether it
	 * is a label or not.*/
	instruction_list_add(&iloc_mode_instr_list);
	iloc_mode_instr_list->opcode = OP_NOP;

	/* count number of instructions. */
	num_instr = 0;
	instruction* tmp = iloc_mode_instr_list;
	while (tmp) {
		++num_instr;
		assert(tmp->next != tmp);
		if (tmp->next == tmp) break;
		tmp = tmp->next;
	}

	/* put all instructions in an array, so it's easier to access them
	* (+ random access! ;)*/
	instr_array = (instruction*)malloc((num_instr + 10) * sizeof(instruction));
	tmp = iloc_mode_instr_list;
	int i = num_instr - 1;
	while (tmp) {
		assert(i >= 0);
		instr_array[i] = *tmp;
		tmp = tmp->next;
		--i;
	}
}

void loop_optimization()
{
	printf("\nLoop optimization:\n\n");
	int i;
	int j;
	for(i = 0; i < bb_graph->num_nodes; i++)
	{
		bb_node_t* node = bb_graph->nodes[i];
		for(j = 0; j < node->num_next; j++)
		{
			if(check_node_domination(node->next[j], node))
			{
				bb_loop_t* detected_loop = (bb_loop_t*) malloc(sizeof(bb_loop_t));
				detected_loop->start_block = node->next[j];
				detected_loop->jump_block = node;

				if(detected_loop->jump_block->num_next == 1) 	// While type of loop
				{
					detected_loop->exit_block = detected_loop->start_block->next[1];
				}
				else				// Do-While type of loop
				{
					detected_loop->exit_block = detected_loop->jump_block->next[1];
				}

				print_loop(detected_loop);
				optimize_loop(detected_loop);
				free(detected_loop);
			}
		}
	}
}

void print_loop(bb_loop_t* loop)
{
	printf("Loop start:\n");
	print_instruction(&instr_array[loop->start_block->first_instr_index]);
	printf("\nLoop end:\n");
	print_instruction(&instr_array[loop->jump_block->last_instr_index]);
	printf("Loop exit: ");
	print_instruction(&instr_array[loop->exit_block->first_instr_index]);
	printf("\n");
	printf("=======================\n");
}

void optimize_loop(bb_loop_t* loop)
{
	// TO BE CONTINUED
}

int check_node_domination(bb_node_t* dominator, bb_node_t* dominated)
{
	int i;
	for(i = 0; i < bb_graph->num_nodes; i++)
		if(tree[i]->block == dominator)
			return check_node_in_tree(tree[i], dominated);
	return 0;
}

int check_node_in_tree(dom_tree_t* root, bb_node_t* node)
{
	if(root->block == node)
	{
		return 1;
	}
	else
	{
		int found = 0;
		int i;
		for(i = 0; i < root->num_children; i++)
			found = found || check_node_in_tree(root->children[i], node);
		return found;
	}
}

/* Dominators tree:
	- How will it be organized?
	- What data structure will be used? */

void generate_dom_tree() {

	bb_node_t* starting_node = bb_graph->nodes[0];
	bb_node_t* current_node;
	tree = (dom_tree_t**) malloc(sizeof(dom_tree_t*) * bb_graph->num_nodes);
	tree[0] = (dom_tree_t*) malloc(sizeof(dom_tree_t) * bb_graph->num_nodes);

	int alloc = 0;
	for (alloc = 1; alloc < bb_graph->num_nodes; ++alloc)
		tree[alloc] = tree[alloc - 1] + 1;
	
	int i;
	for(i = 0; i < bb_graph->num_nodes; i++)
	{
		int num_children = 0;
		current_node = bb_graph->nodes[i];
		tree[i]->block = current_node;
		
		int j;
		// Define how many nodes this node dominates
		for(j = 0; j < bb_graph->num_nodes; j++)
		{
			if( i!=j )
			{
				if(is_dominated_by(bb_graph->nodes[j], bb_graph->nodes[i], bb_graph->nodes[0], NULL))
				{
					int found = 0;
				
					int k;
					for(k = 0; k < bb_graph->num_nodes; k++)
						if(i != j && j != k && i != k)
							if(is_dominated_by(bb_graph->nodes[j], bb_graph->nodes[k], bb_graph->nodes[0], NULL) &&
								is_dominated_by(bb_graph->nodes[k], bb_graph->nodes[i], bb_graph->nodes[0], NULL) )
									found = 1;
					if(found == 0)
						num_children ++;
				}
			}
		}

		tree[i]->num_children = num_children;
		tree[i]->children = (dom_tree_t**) malloc(sizeof(dom_tree_t*) * num_children);

		int n = 0;
		for(j = 0; j < bb_graph->num_nodes; j++)
		{
			if(i != j)
			{
				if(is_dominated_by(bb_graph->nodes[j], bb_graph->nodes[i], bb_graph->nodes[0], NULL))
				{
					int found = 0;

					int k;
					for(k = 0; k < bb_graph->num_nodes; k++)
						if(j != k && i != k)
							if(is_dominated_by(bb_graph->nodes[j], bb_graph->nodes[k], bb_graph->nodes[0], NULL) &&
							   is_dominated_by(bb_graph->nodes[k], bb_graph->nodes[i], bb_graph->nodes[0], NULL) )
								found = 1;

					if(found == 0)
					{
						tree[i]->children[n] = tree[j];
						n++;
					}
				}
			}
		}
	}

	dom_tree = tree[0];

}

int node_list_contains(bb_node_list_t* list, bb_node_t* node)
{
	bb_node_list_t* current = list;
	while(current != NULL)
	{
		if(current->node == node)
			return 1;

		current = current->next;
	}
	return 0;
}

int is_dominated_by(bb_node_t* current, bb_node_t* target, bb_node_t* start, bb_node_list_t* visited)
{
	if(current == target)
	{
		return 1;
	}
	else if(current == start)
	{
		return 0;
	}
	else
	{
		int on_path = 1;
		
		bb_node_list_t* new_ele = (bb_node_list_t*) malloc(sizeof(bb_node_list_t));
		new_ele->node = current;
		new_ele->next = visited;

		int i;
		for(i = 0; i < current->num_previous; i++)
		{
			if( !node_list_contains(new_ele, current->previous[i] ) )
			{
				bb_node_list_t* new_ele_p = (bb_node_list_t*) malloc(sizeof(bb_node_list_t));
				new_ele_p->node = current->previous[i];
				new_ele_p->next = new_ele;
				on_path = on_path && is_dominated_by(current->previous[i], target, start, new_ele_p);
				free(new_ele_p);
			}
		}
		
		free(new_ele);

		return on_path;
	}
}

void generate_bb_graph() {
	int i, j, k;

	instruction* iarr = instr_array; /* just an alias. */

	/*
	* marked instructions are the ones that will be the limits of the basic
	* blocks (leaders).
	*
	* marked instructions are:
	* - the first instruction of the program.
	* - any instruction right after a jump.
	* - any instruction that is the destination of a jump.
	* 
	* instr_array[i].marked = 1 if instruction i is marked.
	* instr_array[i].marked = 0 if instruction i is not marked. 
	* 
	* */

	/* 
	 * next_instr[i] contains the list of instructions that go after 
	 * instr_array[i]. num_next[i] is the NUMBER of instructions in the list,
	 * and next_instr[i][0 .. n-1] are the n instructions that go after i. 
	 * */
	int** next_instr = (int**)malloc((num_instr + 10) * sizeof(int*));
	int* num_next = (int*)malloc((num_instr + 10) * sizeof(int));
	memset(next_instr, 0, num_instr * sizeof(int));
	memset(num_next, 0, num_instr * sizeof(int));
	
	/* iterate to find basic blocks. */
	for (i = 0; i < num_instr; ++i) {
		next_instr[i] = (int*)malloc(10 * sizeof(int));
		if (iarr[i].opcode == OP_JUMP ||
			iarr[i].opcode == OP_JUMP_I ||
			iarr[i].opcode == OP_CBR) {			
			memset(next_instr[i], 0, 10 * sizeof(int));

			assert(iarr[i].tgt_op_1 != NULL && "JUMP/CBR must have tgt1.");

			/* mark the instruction after the jump/cbr. */
			if (i != num_instr - 1) {
				iarr[i + 1].marked = 1;				
			}

			/* mark destination 1 of jump/cbr */
			int j;
			for (j = 0; j < num_instr; ++j) {
				if (iarr[j].label == NULL) continue;
				if (strcmp(iarr[j].label, iarr[i].tgt_op_1) == 0) {
					iarr[j].marked = 1;					
					next_instr[i][num_next[i]] = j;
					++num_next[i];
					break;
				}
			}

			/* mark destination 2 of cbr */
			if (iarr[i].opcode == OP_CBR) {
				assert(iarr[i].tgt_op_2 != NULL && "CBR must have tgt1 and tgt2");
				for (j = 0; j < num_instr; ++j) {
					if (iarr[j].label == NULL) continue;
					if (strcmp(iarr[j].label, iarr[i].tgt_op_2) == 0) {
						iarr[j].marked = 1;
						next_instr[i][num_next[i]] = j;
						++num_next[i];
						break;
					}
				}
			}
		} else { /* not a jump. next is the very next instruction. */
			if (i != num_instr - 1) {
				next_instr[i][num_next[i]] = i + 1;
				++num_next[i];
			}
		}
	}

	/* first and last instr are always marked. */
	iarr[0].marked = 1;
	iarr[num_instr - 1].marked = 1;

	int num_marked_instr = 0;
	for (i = 0; i < num_instr; ++i) if (iarr[i].marked) ++num_marked_instr;
	assert(num_marked_instr > 0);

	/*for (i = 0; i < num_instr; ++i) {
		printf("%d  ", i + 1);
		print_instruction(&instr_array[i]);
	}

	printf("\n num_marked_instr: %d\n", num_marked_instr);*/

	/*
	 *considering the last instruction is not necessarily marked, but the first
	 * instruction IS necessarily marked, 
	 * num basic blocks = num marked instructions. 
	 * */
	int num_basic_blocks = num_marked_instr - 1;

	/* 
	 * allocate memory for the basic blocks graph. 
	 * */
	bb_graph = (bb_graph_t*)malloc(sizeof(bb_graph_t));
	bb_graph->num_nodes = num_basic_blocks;
	bb_graph->nodes = (bb_node_t**)malloc((num_basic_blocks + 10) * sizeof(bb_node_t*));
	memset(bb_graph->nodes, 0, sizeof((num_basic_blocks + 10) * sizeof(bb_node_t*)));
	for (i = 0; i < num_basic_blocks; ++i) {
		bb_graph->nodes[i] = (bb_node_t*)malloc(sizeof(bb_node_t));
		memset(bb_graph->nodes[i], 0, sizeof(bb_node_t));
		bb_graph->nodes[i]->next = (bb_node_t**)malloc(num_basic_blocks * sizeof(bb_node_t*));
		bb_graph->nodes[i]->previous = (bb_node_t**)malloc(num_basic_blocks * sizeof(bb_node_t*));
	}

	int node_index = 0;
	bb_graph->nodes[0]->first_instr_index = 0;
	for (i = 1; i < num_instr && node_index < num_basic_blocks; ++i) {
		if (iarr[i].marked) {
			if (node_index != num_basic_blocks - 1) {
				bb_graph->nodes[node_index]->last_instr_index = i - 1;
				bb_graph->nodes[node_index + 1]->first_instr_index = i;
			} 
			++node_index;
		}
	}

	/*printf("i = %d, num_instr = %d,"
		"# basic blocks = %d, node_index = %d\n", 
		i, num_instr, num_basic_blocks, node_index);*/

	/* last index of last block is the last instruction. */
	assert(node_index == num_basic_blocks);
	assert(i == num_instr);
	
	bb_graph->nodes[node_index - 1]->last_instr_index = num_instr - 1;

	/*printf("\n");
	for (i = 0; i < num_basic_blocks; ++i) {
		printf("bb #%d: %d-%d\n", i + 1, bb_graph->nodes[i]->first_instr_index + 1,
			bb_graph->nodes[i]->last_instr_index + 1);
	}*/

	/* 
	 * setup next and previous nodes on graph.
	 * */
	for (i = 0; i < num_basic_blocks; ++i) {
		bb_node_t* b = bb_graph->nodes[i];
		/* for every 'next_instr' in b->last_instr_index, find the node which
		 * contains that 'next' as 'first_instr_index'. */
		for (j = 0; j < num_next[b->last_instr_index]; ++j) {			
			/*printf("searching for next %d of %d\n", 
				next_instr[b->last_instr_index][j] + 1, b->last_instr_index + 1);*/
			for (k = 0; k < num_basic_blocks; ++k) {
				bb_node_t* mnb = bb_graph->nodes[k];
				/*printf("bb_graph->nodes[%d]->first_instr_index = %d\n", k + 1, 
					mnb->first_instr_index + 1);*/
				if (mnb->first_instr_index == next_instr[b->last_instr_index][j]) {
					b->next[b->num_next] = mnb;
					++b->num_next;
					mnb->previous[mnb->num_previous] = b;
					++mnb->num_previous;
					break; /* break cuz there should be only 1 next block for
						   every next_instr;*/
				}				
			}
		}
		/*printf("b->num_next = %d, num_next[b->last_instr_index] = %d\n",
			b->num_next , num_next[b->last_instr_index]);*/
		assert(b->num_next == num_next[b->last_instr_index]);
	}

	//printf("\n");
	//for (i = 0; i < num_basic_blocks; ++i) {
	//	printf("next[%d-%d] = ", bb_graph->nodes[i]->first_instr_index + 1,
	//		bb_graph->nodes[i]->last_instr_index + 1);
	//	for (j = 0; j < bb_graph->nodes[i]->num_next; ++j) {
	//		printf("%d-%d ", bb_graph->nodes[i]->next[j]->first_instr_index + 1,
	//			bb_graph->nodes[i]->next[j]->last_instr_index + 1);
	//	}
	//	printf("\n");
	//}

	//printf("\n");
	//for (i = 0; i < num_basic_blocks; ++i) {
	//	printf("previous[%d-%d] = ", bb_graph->nodes[i]->first_instr_index + 1,
	//		bb_graph->nodes[i]->last_instr_index + 1);
	//	for (j = 0; j < bb_graph->nodes[i]->num_previous; ++j) {
	//		printf("%d-%d ", bb_graph->nodes[i]->previous[j]->first_instr_index + 1,
	//			bb_graph->nodes[i]->previous[j]->last_instr_index + 1);
	//	}
	//	printf("\n");
	//}

	for (i = 0; i < num_instr; ++i) {
		if (next_instr[i]) free(next_instr[i]);
	}
	if (next_instr) free(next_instr);
	if (num_next) free(num_next);
}

