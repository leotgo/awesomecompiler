#include "cc_iloc.h"


void instruction_list_add(instruction** instr_list)
{
	instruction* new = (instruction*)malloc(sizeof(instruction));
	new->opcode = OP_NOP;
	new->next = *instr_list;
	new->tgt_reg_1 = NULL;
	new->tgt_reg_2 = NULL;
	new->src_reg_1 = NULL;
	new->src_reg_2 = NULL;
	new->valI = NULL;
	*instr_list = new;
}

void print_instruction_list(instruction* instr_list)
{
	recursive_parse(instr_list);
}

void recursive_parse(instruction* list)
{
	if (list->next == NULL) return;
	recursive_parse(list->next);
	
	print_instruction(list);
}

void print_instruction(instruction* list)
{
	
	switch(list->opcode)
	{
		case OP_ADD:
			printf("add");
			break;
			
		case OP_SUB:
			printf("sub");
			break;
			
		case OP_DIV:
			printf("div");
			break;
			
		case OP_ADD_I:
			printf("addI");
			break;
			
		case OP_SUB_I:
			printf("subI");
			break;
			
		case OP_R_SUB_I:
			printf("rsubI");
			break;
			
		case OP_MULT_I:
			printf("multI");
			break;
			
		case OP_DIV_I:
			printf("divI");
			break;
			
		case OP_R_DIV_I:
			printf("rdivI");
			break;
			
		case OP_L_SHIFT:
			printf("lshift");
			break;
			
		case OP_L_SHIFT_I:
			printf("lshiftI");
			break;
			
		case OP_R_SHIFT:
			printf("rshift");
			break;
			
		case OP_R_SHIFT_I:
			printf("rshiftI");
			break;
			
		case OP_AND:
			printf("and");
			break;
			
		case OP_AND_I:
			printf("andI");
			break;
			
		case OP_OR:
			printf("or");
			break;
			
		case OP_OR_I:
			printf("orI");
			break;
			
		case OP_XOR:
			printf("xor");
			break;
			
		case OP_XOR_I:
			printf("xorI");
			break;
			
		case OP_LOAD_I:
			printf("loadI");
			break;
			
		case OP_LOAD:
			printf("load");
			break;
			
		case OP_LOAD_A_I:
			printf("loadAI");
			break;
			
		case OP_LOAD_A_O:
			printf("loadAO");
			break;
			
		case OP_CLOAD:
			printf("cload");
			break;
			
		case OP_CLOAD_A_I:
			printf("cloadAI");
			break;
			
		case OP_CLOAD_A_O:
			printf("cloadAO");
			break;
			
		case OP_STORE:
			printf("store");
			break;
			
		case OP_STORE_A_I:
			printf("storeAI");
			break;
			
		case OP_STORE_A_O:
			printf("storeAO");
			break;
			
		case OP_C_STORE:
			printf("cstore");
			break;
			
		case OP_C_STORE_A_I:
			printf("cstoreAI");
			break;
			
		case OP_C_STORE_A_O:
			printf("cstoreAO");
			break;
			
		case OP_I_2_I:
			printf("i2i");
			break;
			
		case OP_C_2_C:
			printf("c2c");
			break;
			
		case OP_C_2_I:
			printf("c2i");
			break;
			
		case OP_I_2_C:
			printf("i2c");
			break;
			
		case OP_JUMP_I:
			printf("jumpI");
			break;
			
		case OP_JUMP:
			printf("jump");
			break;
			
		case OP_CMP_LT:
			printf("cmp_LT");
			break;
			
		case OP_CMP_LE:
			printf("cmp_LE");
			break;
			
		case OP_CMP_EQ:
			printf("cmp_EQ");
			break;
			
		case OP_CMP_GE:
			printf("cmp_GE");
			break;
			
		case OP_CMP_GT:
			printf("cmp_GT");
			break;
			
		case OP_CMP_NE:
			printf("cmp_NE");
			break;
	}	
	
	
	
	
}

void instruction_list_destroy(instruction* instr_list)
{
	while (instr_list != NULL) 
	{
		instruction* y = instr_list;
		instr_list = instr_list->next;

		/* Guys,
		 * 
		 * Check whether we really don't have to free the fields in instr:
		 * 
		 * tgt_reg,
		 * src_reg, 
		 * valI ???
		 * 
		 * In theory, we shouldn't have, since tgt_reg and src_reg strings 
		 * should be freed on cc_register's pool. But just in case, we should
		 * keep this in mind!
		 * */

		free(y);
	}
}
