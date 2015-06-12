#include "cc_bb_graph.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void free_bb_graph(bb_graph_t* g) {
	if (g) {
		int i;
		if (g->nodes) {
			assert(g->num_nodes > 0);
			for (i = 0; i < g->num_nodes; ++i) {
				if (g->nodes[i]) {
					if (g->nodes[i]->next) free(g->nodes[i]->next);
					if (g->nodes[i]->previous) free(g->nodes[i]->previous);
					free(g->nodes[i]);
				}
			}
			free(g->nodes);
		}
		free(g);
	}
}
