#include "cc_iloc.h"



void instruction_list_add(struct instruction* instr)
{
	instruction_list* new = (instruction_list*)malloc(sizeof(instruction_list));
	new->instr = instr;
	new->next = instr_list;
}

void print_instruction_list()
{
	recursive_parse(instr_list);
}

void recursive_parse(instruction_list* list)
{
	recursive_parse(list->next);
	
	// TODO print here
}

void instruction_list_destroy()
{
	while (instr_list != NULL) 
	{
		instruction_list* y = instr_list;
		instr_list = instr_list->next;
		free(y);
	}
}
