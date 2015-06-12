#include "cc_iloc_parser.h"
#include "cc_str_pool.h"

#define CHARMATCH "[a-zA-Z0-9_]"

void iloc_parse(FILE* f) {
	assert(f);
	char* buf = str_pool_add((char*)malloc(1000 * sizeof(char)));
	char* buf2 = str_pool_add((char*)malloc(1000 * sizeof(char)));
	while (fgets(buf, 1000, f)) {
		
		char* label = str_pool_add((char*)malloc(800 * sizeof(char)));
		char* opcode = str_pool_add((char*)malloc(100 * sizeof(char)));
		char* src1 = str_pool_add((char*)malloc(100 * sizeof(char)));
		char* src2 = str_pool_add((char*)malloc(100 * sizeof(char)));
		char* tgt1 = str_pool_add((char*)malloc(100 * sizeof(char)));
		char* tgt2 = str_pool_add((char*)malloc(100 * sizeof(char)));
		
		instruction* ins = NULL;
		instruction_list_add(&ins);

		if (sscanf(buf, " %[^:]%[:] ", label, buf2) == 2) {
			ins->label = label;
			if (!fgets(buf, 1000, f)) {
				iloc_mode_instr_list = instruction_list_merge(
					&iloc_mode_instr_list, &ins);
				break;
			}
		}

		if (sscanf(buf, " %"CHARMATCH" %"CHARMATCH" , %"CHARMATCH" => %"CHARMATCH" ", 
			opcode, src1, src2, tgt1) == 4
			|| sscanf(buf, " %"CHARMATCH" %"CHARMATCH" , %"CHARMATCH" -> %"CHARMATCH" ", 
			opcode, src1, src2, tgt1) == 4) {
			/* 2 src and 1 target operands*/
			ins->opcode = get_opcode_from_string(opcode);
			ins->src_op_1 = src1;
			ins->src_op_2 = src2;
			ins->tgt_op_1 = tgt1;
		} else if (sscanf(buf, " %"CHARMATCH" %"CHARMATCH" => %"CHARMATCH" , %"CHARMATCH" ",
			opcode, src1, tgt1, tgt2) == 4
			|| sscanf(buf, " %"CHARMATCH" %"CHARMATCH" -> %"CHARMATCH" , %"CHARMATCH" ",
			opcode, src1, tgt1, tgt2) == 4) {
			/* 1 src and 2 target operands*/
			ins->opcode = get_opcode_from_string(opcode);
			ins->src_op_1 = src1;
			ins->tgt_op_1 = tgt1;
			ins->tgt_op_2 = tgt2;
		} else if (sscanf(buf, " %"CHARMATCH" %"CHARMATCH" => %"CHARMATCH" ",
			opcode, src1, tgt1) == 3
			|| sscanf(buf, " %"CHARMATCH" %"CHARMATCH" -> %"CHARMATCH" ",
			opcode, src1, tgt1) == 3) {
			/* 1 src and 1 target operands*/
			ins->opcode = get_opcode_from_string(opcode);
			ins->src_op_1 = src1;
			ins->tgt_op_1 = tgt1;
		} else if (sscanf(buf, " %"CHARMATCH" => %"CHARMATCH" ", opcode, tgt1) == 2
			|| sscanf(buf, " %"CHARMATCH" -> %"CHARMATCH" ", opcode, tgt1) == 2) {
			/* 0 src and 1 target operands, setinha -> */
			ins->opcode = get_opcode_from_string(opcode);
			ins->tgt_op_1 = tgt1;
		} else if (sscanf(buf, " %s ", buf2) == 1) {
			/* nop */
			ins->opcode = OP_NOP;
		} else {
			printf("Error: ill-formed ILOC input file. Aborting.\n");
			exit(-1);
		}

		iloc_mode_instr_list = instruction_list_merge(&iloc_mode_instr_list, &ins);
	}
}

int get_opcode_from_string(char* str) {
	if (strcmp(str, "add") == 0) {
		return OP_ADD;
	} else if (strcmp(str, "sub") == 0) {
		return OP_SUB;
	} else if (strcmp(str, "mult") == 0) {
		return OP_MULT;
	} else if (strcmp(str, "div") == 0) {
		return OP_DIV;
	} else if (strcmp(str, "addI") == 0) {
		return OP_ADD_I;
	} else if (strcmp(str, "subI") == 0) {
		return OP_SUB_I;
	} else if (strcmp(str, "rsubI") == 0) {
		return OP_R_SUB_I;
	} else if (strcmp(str, "multI") == 0) {
		return OP_MULT_I;
	} else if (strcmp(str, "divI") == 0) {
		return OP_DIV_I;
	} else if (strcmp(str, "rdivI") == 0) {
		return OP_R_DIV_I;
	} else if (strcmp(str, "lshift") == 0) {
		return OP_L_SHIFT;
	} else if (strcmp(str, "lshiftI") == 0) {
		return OP_L_SHIFT_I;
	} else if (strcmp(str, "rshift") == 0) {
		return OP_R_SHIFT;
	} else if (strcmp(str, "rshiftI") == 0) {
		return OP_R_SHIFT_I;
	} else if (strcmp(str, "and") == 0) {
		return OP_AND;
	} else if (strcmp(str, "andI") == 0) {
		return OP_AND_I;
	} else if (strcmp(str, "or") == 0) {
		return OP_OR;
	} else if (strcmp(str, "orI") == 0) {
		return OP_OR_I;
	} else if (strcmp(str, "xor") == 0) {
		return OP_XOR;
	} else if (strcmp(str, "xorI") == 0) {
		return OP_XOR_I;
	} else if (strcmp(str, "loadI") == 0) {
		return OP_LOAD_I;
	} else if (strcmp(str, "load") == 0) {
		return OP_LOAD;
	} else if (strcmp(str, "loadAI") == 0) {
		return OP_LOAD_A_I;
	} else if (strcmp(str, "loadAO") == 0) {
		return OP_LOAD_A_O;
	} else if (strcmp(str, "cload") == 0) {
		return OP_CLOAD;
	} else if (strcmp(str, "cloadAI") == 0) {
		return OP_CLOAD_A_I;
	} else if (strcmp(str, "cloadAO") == 0) {
		return OP_CLOAD_A_O;
	} else if (strcmp(str, "store") == 0) {
		return OP_STORE;
	} else if (strcmp(str, "storeAI") == 0) {
		return OP_STORE_A_I;
	} else if (strcmp(str, "storeAO") == 0) {
		return OP_STORE_A_O;
	} else if (strcmp(str, "cstore") == 0) {
		return OP_C_STORE;
	} else if (strcmp(str, "cstoreAI") == 0) {
		return OP_C_STORE_A_I;
	} else if (strcmp(str, "cstoreAO") == 0) {
		return OP_C_STORE_A_O;
	} else if (strcmp(str, "i2i") == 0) {
		return OP_I_2_I;
	} else if (strcmp(str, "c2c") == 0) {
		return OP_C_2_C;
	} else if (strcmp(str, "c2i") == 0) {
		return OP_C_2_I;
	} else if (strcmp(str, "i2c") == 0) {
		return OP_I_2_C;
	} else if (strcmp(str, "jumpI") == 0) {
		return OP_JUMP_I;
	} else if (strcmp(str, "jump") == 0) {
		return OP_JUMP;
	} else if (strcmp(str, "cmp_LT") == 0) {
		return OP_CMP_LT;
	} else if (strcmp(str, "cmp_LE") == 0) {
		return OP_CMP_LE;
	} else if (strcmp(str, "cmp_EQ") == 0) {
		return OP_CMP_EQ;
	} else if (strcmp(str, "cmp_GE") == 0) {
		return OP_CMP_GE;
	} else if (strcmp(str, "cmp_GT") == 0) {
		return OP_CMP_GT;
	} else if (strcmp(str, "cmp_NE") == 0) {
		return OP_CMP_NE;
	} else if (strcmp(str, "cbr") == 0) {
		return OP_CBR;
	} else if (strcmp(str, "nop") == 0) {
		return OP_NOP;
	} 
	return OP_NOP;
}

