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
			printf("add %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_SUB:
			printf("sub %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
		
		case OP_MULT:
			printf("mult %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_DIV:
			printf("div %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_ADD_I:
			printf("addI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_SUB_I:
			printf("subI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_R_SUB_I:
			printf("rsubI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_MULT_I:
			printf("multI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_DIV_I:
			printf("divI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_R_DIV_I:
			printf("rdivI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_L_SHIFT:
			printf("lshift %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_L_SHIFT_I:
			printf("lshiftI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_R_SHIFT:
			printf("rshift %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_R_SHIFT_I:
			printf("rshiftI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_AND:
			printf("and %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_AND_I:
			printf("andI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_OR:
			printf("or %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_OR_I:
			printf("orI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_XOR:
			printf("xor %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_XOR_I:
			printf("xorI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_LOAD_I:
			printf("loadI %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_LOAD:
			printf("load %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_LOAD_A_I:
			printf("loadAI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_LOAD_A_O:
			printf("loadAO %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CLOAD:
			printf("cload %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_CLOAD_A_I:
			printf("cloadAI %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CLOAD_A_O:
			printf("cloadAO %s, %s => %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_STORE:
			printf("store %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_STORE_A_I:
			printf("storeAI %s => %s, %s", list->src_op_1, list->tgt_op_1, list->tgt_op_2);
			break;
			
		case OP_STORE_A_O:
			printf("storeAO %s => %s, %s", list->src_op_1, list->tgt_op_1, list->tgt_op_2);
			break;
			
		case OP_C_STORE:
			printf("cstore %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_C_STORE_A_I:
			printf("cstoreAI %s => %s, %s", list->src_op_1, list->tgt_op_1, list->tgt_op_2);
			break;
			
		case OP_C_STORE_A_O:
			printf("cstoreAO %s => %s, %s", list->src_op_1, list->tgt_op_1, list->tgt_op_2);
			break;
			
		case OP_I_2_I:
			printf("i2i %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_C_2_C:
			printf("c2c %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_C_2_I:
			printf("c2i %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_I_2_C:
			printf("i2c %s => %s", list->src_op_1, list->tgt_op_1);
			break;
			
		case OP_JUMP_I:
			printf("jumpI -> %s", list->tgt_op_1);
			break;
			
		case OP_JUMP:
			printf("jump -> %s", list->tgt_op_1);
			break;
			
		case OP_CMP_LT:
			printf("cmp_LT %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CMP_LE:
			printf("cmp_LE %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CMP_EQ:
			printf("cmp_EQ %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CMP_GE:
			printf("cmp_GE %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CMP_GT:
			printf("cmp_GT %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CMP_NE:
			printf("cmp_NE %s, %s -> %s", list->src_op_1, list->src_op_2, list->tgt_op_1);
			break;
			
		case OP_CBR:
			printf("cbr %s -> %s, %s", list->src_op_1, list->tgt_op_1, list->tgt_op_2); 
			break;
	}	
	
	printf("\n");
	
	
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
