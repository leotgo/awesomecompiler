//#ifndef __ILOC_H
//#define __ILOC_H

#include <stdlib.h>
#include <stdio.h>
#include "main.h"

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
#define OP_I_2_I	 	32
#define OP_C_2_C		33
#define OP_C_2_I	 	34
#define OP_I_2_C	 	35
#define OP_JUMP_I	 	36
#define OP_JUMP		 	37
#define OP_CMP_LT	 	38
#define OP_CMP_LE	 	39
#define OP_CMP_EQ	 	40
#define OP_CMP_GE	 	41
#define OP_CMP_GT	 	42
#define OP_CMP_NE	 	43

typedef struct instruction
{
	char* opcode;		// The opcode for this instruction

	char* tgt_reg;		// The target register for the instruction
	char* src_reg_1;	// The first source register for instruction. May be empty.
	char* src_reg_2;	// The second source register for the instruction. May also be empty.

	void* valI;		// Pointer to immediate value for some types of operations

} instruction;


// reversed list of instructions. 
//To print instructions in correct order must use function print_instructions
typedef struct instruction_list 
{
	struct instruction* instr;
	struct instruction_list* next;
	
} instruction_list;

instruction_list* instr_list = NULL;

void instruction_list_add(struct instruction* instr);

void print_instruction_list();

void recursive_parse(instruction_list* list);

void instruction_list_destroy();

