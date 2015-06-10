#ifndef __CALL_STACK
#define __CALL_STACK

#include <stdlib.h>
#include <stdio.h>

typedef struct activation_frame
{
	int temporary;
	
	int local_variables;
	
	int machine_state;
	
	int static_link;
	
	int dynamic_link;
	
	void* returned_value;	// valor de retorno. pode ser int, float, char ou string.
	
	void* arguments;		// guarda argumentos da função
	
	char* return_address; 	// guarda label de retorno


} activation_frame;


void activation_frame_marshall();

void activation_frame_unmarshall();

#endif
