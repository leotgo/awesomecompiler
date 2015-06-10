#ifndef __CALL_STACK
#define __CALL_STACK

#include <stdlib.h>
#include <stdio.h>
#include "cc_gencode.h"
#include "cc_register.h"
#include "cc_tree.h"
#include <assert.h>
#include "cc_str_pool.h"

typedef struct comp_tree_t comp_tree_t;

typedef struct activation_frame
{
	int temporary;
	
	int local_variables;
	
	int machine_state;
	
	char* static_link;	 	// valor de fp
	
	char* dynamic_link;		// valor de sp
	
	char* returned_value;	// registrador com o valor de retorno
	
	void* arguments;		// guarda argumentos da função
	
	char* return_address; 	// guarda label de retorno


} activation_frame;


void activation_frame_marshall(activation_frame* frame, comp_tree_t* node);

void activation_frame_unmarshall();



#endif
