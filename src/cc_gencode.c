#include "cc_gencode.h"
#include "cc_type.h"
#include "cc_register.h"
#include "cc_label.h"
#include "cc_iloc.h"

/* converts the int 'i', into a string (it allocates the necessary memory for 
 * that string).
 * it adds the string to the node pool so we don't ahve to worry about 
 * releasing the memory.*/
char* int_str(int i) {
	/* a 32-bit integer will never exceed 10 digits, so the
	* address string will never be more than 12 digits ;)
	* (just to be safe)
	* */
	char* str = (char*)malloc(12 * sizeof(char));
	memset(str, 0, 12 * sizeof(char));
	sprintf(str, "%d", i);
	str_pool_add(str);
	return str;
}

// The main code generation function. Called over the AST tree root, after it 
// is created.

void generate_code(comp_tree_t* node, char* regdest)
{
	switch(node->type)
	{
		/* Leonardo ********************************* */
		case AST_IF_ELSE:
			break;
		case AST_DO_WHILE:
			break;
		case AST_WHILE_DO:
			break;
		case AST_LOGICO_COMP_DIF:
			break;
		case AST_LOGICO_COMP_IGUAL:
			break;
		case AST_LOGICO_COMP_LE:
			break;
		case AST_LOGICO_COMP_GE:
			break;
		case AST_LOGICO_COMP_L:
			break;
		case AST_LOGICO_COMP_G:
			break;
		case AST_LOGICO_COMP_NEGACAO:
			break;
		/* ****************************************** */

		/* Marina *********************************** */
		case AST_ARIM_SOMA:
			generate_code_operation(node, regdest, OP_ADD); 
			break;
			
		case AST_ARIM_SUBTRACAO:
			generate_code_operation(node, regdest, OP_SUB); 
			break;
			
		case AST_ARIM_MULTIPLICACAO:
			generate_code_operation(node, regdest, OP_MULT); 
			break;
			
		case AST_ARIM_DIVISAO:
			generate_code_operation(node, regdest, OP_DIV); 
			break;
			
		case AST_ARIM_INVERSAO:
			generate_code_operation_negative(node, regdest);
			break;
			
		case AST_LOGICO_E:
			generate_code_operation(node, regdest, OP_AND); 			
			break;
			
		case AST_LOGICO_OU:
			generate_code_operation(node, regdest, OP_OR); 
			break;
			
		/* ****************************************** */
		
		
		/* Alex ************************************* */
		case AST_IDENTIFICADOR:
			/* 
			 * não tem código, mas aqui tem que ver o ponteiro
			 * pro simbolo no contexto e setar node->addr adequadamente. 
			 * */
			node->addr = get_symbol(node)->addr;

			if (regdest) {
				instruction_list_add(&node->instr_list);

				/* we have to use LOADI (immediate load) if node type is a
				* string. the immediate value will be the address. */
				if (get_symbol(node)->type == IKS_STRING) {					
					node->instr_list->opcode = OP_LOAD_I;
					/* first operand: the immediate value */
					node->instr_list->src_op_1 = int_str(node->addr);
					/* target: the destination register */
					node->instr_list->tgt_op_1 = regdest;

				} else {
					/* variable is anything other than a string. load by value. 
					*
					* LOAD_AI of this identifier's value to rdest.
					*
					* LOAD_AI   reg_base   offset   =>   reg_dest 
					* 
					* */
					node->instr_list->opcode = OP_LOAD_A_I;

					/* target is regdest */
					node->instr_list->tgt_op_1 = regdest;

					/* first operand: fp if variable is global,
					 * or rarp if variable is local. */
					if (node->context == main_context) { /* global variable */
						node->instr_list->src_op_1 = reg_fp();
					} else {
						node->instr_list->src_op_1 = reg_arp();
					}

					/* second operand: the immediate value of the variable's
					 * address.*/
					node->instr_list->src_op_2 = int_str(node->addr);
				}
			}
			break;
		case AST_ATRIBUICAO:
			/* casos:
			 * x = 2 + 3
			 * x = y
			 * x = expressao (ex, x = x + y)
			 * x[5] = 5;
			 * x = "alex"
			 * */

			/* generate code for the expression that is being assigned to
			 * the variable. */
			;


			/* generate code for the left side of the attribution. */
			generate_code(node->children[0], NULL);

			/* reg1 will hold the value to be assigned to the memory 
			 * address. */
			char* reg1 = generate_register();

			/* generate code for the right side of the attrib.*/
			generate_code(node->children[1], reg1);

			/* concatenate children's codes. */
			node->instr_list = instruction_list_merge(
				&node->children[0]->instr_list,
				&node->children[1]->instr_list);

			/* addr of the variable that will be assig. */
			int dest_addr = get_symbol(node->children[0])->addr; 

			/*
			 * ALEX, CONTINUAR AQUI 
			 *
			 **/

			break;
		case AST_VETOR_INDEXADO:
			/* casos
			 * v[5]
			 * v[x]
			 * v[x + 5]
			 * v[x, y, z]
			 * v[1, x + y, x + 5]
			 * */
			break;
		/* ****************************************** */

		/* Depois decidimos quem vai fazer ********** */
		case AST_PROGRAMA:
			break;
		case AST_FUNCAO:
			break;
		case AST_LITERAL:
			break;
		case AST_INPUT:
			break;
		case AST_OUTPUT:
			break;
		case AST_RETURN:
			break;
		case AST_BLOCO:
			break;
		/* ****************************************** */
		
		case AST_CHAMADA_DE_FUNCAO:
			/* Do nothing (IN THIS STAGE, WILL NOT BE IMPLEMENTED) */
			break;
	}	
}

void generate_code_operation_negative(comp_tree_t* node, char* regdest)
{
	// register for value of children 0
	char* r1 = generate_register();	
	
	generate_code(node->children[0],r1);
	
	char *zero = (char*) malloc(2);
	strcpy(zero, "0");
	
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_R_SUB_I;
	node->instr_list->src_op_1 = r1;
	node->instr_list->src_op_1 = zero;
	node->instr_list->tgt_op_1 = regdest;
}

void generate_code_operation(comp_tree_t* node, char* regdest, int operation)
{
	// register for value of children 0
	char* r1 = generate_register();	
	// register for value of chldren 1
	char* r2 = generate_register();
	
	generate_code(node->children[0],r1);
	generate_code(node->children[1],r2);
	
	
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = operation;
	node->instr_list->src_op_1 = r1;
	node->instr_list->src_op_1 = r2;
	node->instr_list->tgt_op_1 = regdest;
	
}

void generate_code_if_else(comp_tree_t* node)
{
	// Generate code for IF-ELSE condition test
	char* cond_reg = generate_register();
	generate_code(node->children[0], cond_reg);

	// Generate labels for condition cases TRUE and FALSE
	char* case_true_label = generate_label();
	char* case_false_label = generate_label();	

	// Add BRANCH code based on IF-ELSE condition test
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_CBR;
	node->instr_list->src_op_1 = cond_reg;
	node->instr_list->tgt_op_1 = case_true_label;
	node->instr_list->tgt_op_2 = case_false_label;

	// Add condition true case label
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_true_label;

	// Add condition true case code block	
	generate_code(node->children[1], NULL);
	
	// Add condition false case label
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_false_label;

	// Add condition false case code block, if it exists
	if(node->children[2] != NULL)
		generate_code(node->children[2], NULL);
}

void generate_code_do_while(comp_tree_t* node)
{
	// Generate labels for condition cases TRUE and FALSE
	char* case_true_label = generate_label();
	char* case_false_label = generate_label();

	// Add label to start of DO-WHILE code block
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_true_label;

	// Add instruction list of DO-WHILE code block
	generate_code(node->children[1], NULL);

	// Add instruction list of DO-WHILE condition test
	char* cond_reg = generate_register();
	generate_code(node->children[0], cond_reg);

	// Add BRANCH code based on DO-WHILE condition test
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_CBR;
	node->instr_list->src_op_1 = cond_reg;
	node->instr_list->tgt_op_1 = case_true_label;
	node->instr_list->tgt_op_2 = case_false_label;

	// Add label to case when test is false (which means going to the next instruction)
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_false_label;
}

void generate_code_while_do(comp_tree_t* node)
{
	// Generate labels for condition cases TRUE and FALSE
	char* case_true_label = generate_label();
	char* case_false_label = generate_label();

	// Generate code for WHILE-DO condition test
	char* cond_reg = generate_register();
	generate_code(node->children[0], cond_reg);

	// Add BRANCH code based on WHILE-DO condition test
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_CBR;
	node->instr_list->src_op_1 = cond_reg;
	node->instr_list->tgt_op_1 = case_true_label;
	node->instr_list->tgt_op_2 = case_false_label;

	// Add label to WHILE-DO code block
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_true_label;

	// Add instruction list of WHILE-DO code block
	generate_code(node->children[1], NULL);

	// Add label to case when test is false (which means going to the next instruction)
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_false_label;
}
