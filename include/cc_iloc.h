#ifndef __ILOC_H
#define __ILOC_H

#include <stdlib.h>
#include <stdio.h>

#define OP_ADD 			1
#define OP_SUB 			2
#define OP_MULT 		3
#define OP_DIV 			4
#define OP_ADD_I 		5
#define OP_SUB_I 		6
#define OP_R_SUB_I 		7
#define OP_MULT_I 		8
#define OP_DIV_I 		9
#define OP_R_DIV_I 		10
#define OP_L_SHIFT 		11
#define OP_L_SHIFT_I 	12
#define OP_R_SHIFT 		13
#define OP_R_SHIFT_I 	14
#define OP_AND 			15
#define OP_AND_I 		16
#define OP_OR 			17
#define OP_OR_I 		18
#define OP_XOR 			19
#define OP_XOR_I 		20
#define OP_LOAD_I 		21
#define OP_LOAD 		22
#define OP_LOAD_A_I 	23
#define OP_LOAD_A_O 	24
#define OP_CLOAD 		25
#define OP_CLOAD_A_I 	26
#define OP_CLOAD_A_O 	27
#define OP_STORE 		28
#define OP_STORE_A_I 	29
#define OP_STORE_A_O	30
#define OP_C_STORE 		31
#define OP_C_STORE_A_I	32
#define OP_C_STORE_A_O	33
#define OP_I_2_I	 	34
#define OP_C_2_C		35
#define OP_C_2_I	 	36
#define OP_I_2_C	 	37
#define OP_JUMP_I	 	38
#define OP_JUMP		 	39
#define OP_CMP_LT	 	40
#define OP_CMP_LE	 	41
#define OP_CMP_EQ	 	42
#define OP_CMP_GE	 	43
#define OP_CMP_GT	 	44
#define OP_CMP_NE	 	45
#define OP_CBR			46
#define OP_NOP			47
#define OP_NOT			48 	// we created new iloc operation: r1 => r2; r2 has inverted value

typedef struct instruction
{
	char* label;	
	
	int opcode;		// The opcode for this instruction

	char* tgt_op_1; // The first target register for the instruction. 
	// This one may NOT be empty.
	
	char* tgt_op_2; // The second target register for the instruction. 
	// May be empty.

	char* src_op_1; // The first source register for instruction. 
	// May be empty.

	char* src_op_2; // The second source register for the instruction. 
	// May also be empty.

	struct instruction* next;

} instruction;

/* 
 * adds instr to the beginning of instr_list, and sets the address of that list 
 * to the newly inserted element.
 * ex: instruction_list_add(&instr_list);
 * */
void instruction_list_add(instruction** instr_list);


/* 
 * merges two instruction lists. the input lists can be considered to be both
 * destroyed after this function is called; accessing them after this call
 * will result in undefined behaviour. 
 * 
 * this function returns a new list which is the concatenation of l1 and l2 
 * (where l2 come after l1). 
 * 
 * it is not a problem if one (or both) of them is NULL.
 * */
instruction* instruction_list_merge(instruction** l1, instruction** l2);

void print_instruction_list(instruction* list);

void instruction_list_destroy(instruction* list);

instruction* instruction_list_new();

void recursive_parse(instruction* list);

void print_instruction(instruction* list);

#endif
