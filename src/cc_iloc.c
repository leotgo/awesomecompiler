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
	
	print_instruction(list);
}

void print_instruction(instruction_list* list)
{
	
	switch(list->instr->opcode)
	{
		case OP_ADD:
			printf("add %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_SUB:
			printf("sub %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
		
		case OP_MULT:
			printf("mult %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_DIV:
			printf("div %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
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
			printf("lshift %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_L_SHIFT_I:
			printf("lshiftI");
			break;
			
		case OP_R_SHIFT:
			printf("rshift %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_R_SHIFT_I:
			printf("rshiftI");
			break;
			
		case OP_AND:
			printf("and %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_AND_I:
			printf("andI");
			break;
			
		case OP_OR:
			printf("or %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_OR_I:
			printf("orI");
			break;
			
		case OP_XOR:
			printf("xor %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_XOR_I:
			printf("xorI");
			break;
			
		case OP_LOAD_I:
			printf("loadI");
			break;
			
		case OP_LOAD:
			printf("load %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_LOAD_A_I:
			printf("loadAI");
			break;
			
		case OP_LOAD_A_O:
			printf("loadAO %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CLOAD:
			printf("cload %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_CLOAD_A_I:
			printf("cloadAI");
			break;
			
		case OP_CLOAD_A_O:
			printf("cloadAO %s, %s => %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_STORE:
			printf("store %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_STORE_A_I:
			printf("storeAI");
			break;
			
		case OP_STORE_A_O:
			printf("storeAO %s => %s, %s", list->instr->src_reg_1, list->instr->tgt_reg_1, list->instr->tgt_reg_2);
			break;
			
		case OP_C_STORE:
			printf("cstore %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_C_STORE_A_I:
			printf("cstoreAI");
			break;
			
		case OP_C_STORE_A_O:
			printf("cstoreAO %s => %s, %s", list->instr->src_reg_1, list->instr->tgt_reg_1, list->instr->tgt_reg_2);
			break;
			
		case OP_I_2_I:
			printf("i2i %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_C_2_C:
			printf("c2c %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_C_2_I:
			printf("c2i %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_I_2_C:
			printf("i2c %s => %s", list->instr->src_reg_1, list->instr->tgt_reg_1);
			break;
			
		case OP_JUMP_I:
			printf("jumpI");
			break;
			
		case OP_JUMP:
			printf("jump -> %s", list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_LT:
			printf("cmp_LT %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_LE:
			printf("cmp_LE %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_EQ:
			printf("cmp_EQ %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_GE:
			printf("cmp_GE %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_GT:
			printf("cmp_GT %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CMP_NE:
			printf("cmp_NE %s, %s -> %s", list->instr->src_reg_1, list->instr->src_reg_2, list->instr->tgt_reg_1);
			break;
			
		case OP_CBR:
			printf("cbr");
			break;
	}	
	
	
	
	
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
