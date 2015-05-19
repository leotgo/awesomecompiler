#include "cc_iloc.h"

typedef struct instruction
{
	char* opcode;		// The opcode for this instruction

	char* tgt_reg;		// The target register for the instruction
	char* src_reg_1;	// The first source register for instruction. May be empty.
	char* src_reg_2;	// The second source register for the instruction. May also be empty.

	void* valI;		// Pointer to immediate value for some types of operations

} instruction;
