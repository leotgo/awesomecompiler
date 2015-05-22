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
	
	//printf("gencode %d \n", node->type);
	switch(node->type)
	{
		/* Leonardo ********************************* */
		case AST_IF_ELSE:
			generate_code_if_else(node);
			break;
		case AST_DO_WHILE:
			generate_code_do_while(node);
			break;
		case AST_WHILE_DO:
			generate_code_while_do(node);
			break;
		case AST_LOGICO_COMP_DIF:
			generate_code_comparison(node, regdest, OP_CMP_NE);
			break;
		case AST_LOGICO_COMP_IGUAL:
			generate_code_comparison(node, regdest, OP_CMP_EQ);
			break;
		case AST_LOGICO_COMP_LE:
			generate_code_comparison(node, regdest, OP_CMP_LE);
			break;
		case AST_LOGICO_COMP_GE:
			generate_code_comparison(node, regdest, OP_CMP_GE);
			break;
		case AST_LOGICO_COMP_L:
			generate_code_comparison(node, regdest, OP_CMP_LT);
			break;
		case AST_LOGICO_COMP_G:
			generate_code_comparison(node, regdest, OP_CMP_GT);
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
			generate_code_boolean_operation(node, regdest, OP_AND); 			
			break;
			
		case AST_LOGICO_OU:
			generate_code_boolean_operation(node, regdest, OP_OR); 
			break;
			
		/* ****************************************** */
		
		
		/* Alex ************************************* */
		case AST_IDENTIFICADOR:
			generate_code_identificador(node, regdest);
			break;
			
		case AST_ATRIBUICAO:
			generate_code_atribuicao(node, regdest);

			break;
		case AST_VETOR_INDEXADO:
			generate_code_vetor_indexado(node, regdest);
			break;
		/* ****************************************** */

		/* Depois decidimos quem vai fazer ********** */
		case AST_PROGRAMA:
			generate_children_code(node, regdest);
			break;
			
		case AST_FUNCAO:
			generate_children_code(node, regdest);
			break;
		case AST_LITERAL:
			generate_code_literal(node, regdest);
			break;
		case AST_INPUT:
			// nao tem que fazer
			break;
		case AST_OUTPUT:
			// nao tem que fazer
			break;
		case AST_RETURN:
			// ACHO que nao tem que fazer ainda
			break;
		case AST_BLOCO:
			// acho que nao precisa fazer nada?
			break;
		/* ****************************************** */
		
		case AST_CHAMADA_DE_FUNCAO:
			/* Do nothing (IN THIS STAGE, WILL NOT BE IMPLEMENTED) */
			break;
	}	
	
	if(node->type == AST_PROGRAMA)
	{
		// calls function to print instruction list 
		
		print_instruction_list(node->instr_list);
		
	}
	
}

char* get_reg_fp_or_arp(comp_tree_t* node) {
	comp_context_symbol_t* ss = get_symbol(node);
	if (context_find_identifier(main_context, ss->key) != NULL) {
		return reg_fp();
	} else {
		return reg_arp();
	}
}

void generate_code_vetor_indexado(comp_tree_t* node, char* regdest) {
	/* casos
	* v[5]
	* v[x]
	* v[x + 5]
	* v[x, y, z]
	* v[1, x + y, x + 5]
	*
	* child 1: v
	* child 2: lista de argumentos
	*
	* */
	if (node->num_children != 2) {
		perror("generate_code() for AST_VETOR_INDEXADO: node should "
			"have 2 children but does not.");
		exit(EXIT_FAILURE);
	}

	/* we set the context for this node to be the same context
	* where v was assigned. */
	node->context = node->children[0]->context;

	if (regdest != NULL) {
		/* if regdest is not null, we must load the value to
		* the register regdest. */

		/*in order to do that, first we must compute the address
		*of the vector indexing: */
		char* regaddr = generate_register();
		instruction* ii = calculate_vector_indexing_address(node, regaddr);
		node->instr_list = instruction_list_merge(&ii, &node->instr_list);

		/* having the address, we must do a load_AO to regdest */
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_LOAD_A_O;
		node->instr_list->src_op_1 = get_reg_fp_or_arp(node);
		node->instr_list->src_op_2 = regaddr;
		node->instr_list->tgt_op_1 = regdest;
	}
}

void generate_code_atribuicao(comp_tree_t* node, char* regdest) {
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


	char* reg_offset = generate_register();

	/* address of the variable that will be assigned. */
	if (node->children[0]->type == AST_VETOR_INDEXADO) {
		instruction* ii = calculate_vector_indexing_address(node->children[0], reg_offset);
		node->instr_list = instruction_list_merge(&node->instr_list, &ii);
	} else {
		int dest_addr = get_symbol(node->children[0])->addr;
		instruction* ii = instruction_list_new();
		ii->opcode = OP_LOAD_I;
		ii->src_op_1 = int_str(dest_addr);
		ii->tgt_op_1 = reg_offset;
		node->instr_list = instruction_list_merge(&node->instr_list, &ii);
	}

	instruction_list_add(&node->instr_list);

	node->instr_list->opcode = OP_STORE_A_O;
	node->instr_list->src_op_1 = reg1;
	node->instr_list->tgt_op_1 = get_reg_fp_or_arp(node->children[0]);

	node->instr_list->tgt_op_2 = reg_offset;

	if (regdest) {
		/* if there is a destination register (because we're doing something 
		 * like  x = y = 10; ), copy reg1 to regdets. 
		 * */
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_I_2_I;
		node->instr_list->src_op_1 = reg1;
		node->instr_list->tgt_op_1 = regdest;
	}
}

void generate_code_identificador(comp_tree_t* node, char* regdest) {
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
			* */
			node->instr_list->opcode = OP_LOAD_A_I;

			/* target is regdest */
			node->instr_list->tgt_op_1 = regdest;

			/* first operand: fp if variable is global,
			* or rarp if variable is local. */
			node->instr_list->src_op_1 = get_reg_fp_or_arp(node);

			/* second operand: the immediate value of the variable's
			* address.*/
			node->instr_list->src_op_2 = int_str(node->addr);
		}
	}
}

instruction* calculate_vector_indexing_address(const comp_tree_t* node, 
	char* regdest_address) {

	if (regdest_address == NULL || node == NULL) {
		perror("We should never call generate_code_vector_indexing() "
			"with NULL parameters.");
		exit(EXIT_FAILURE);
	} else if (node->type != AST_VETOR_INDEXADO) {
		perror("Node given to generate_code_vector_indexing() is not of "
			"type AST_VETOR_INDEXADO. This is wrong!");
		exit(EXIT_FAILURE);
	}

	static int label_count = 0;

	instruction* ii = NULL;

	instruction_list_add(&ii);
	ii->opcode = OP_NOP;
	ii->label = (char*)malloc(256 * sizeof(char));
	sprintf(ii->label, "vector_indexing_%d_START", label_count);
	str_pool_add(ii->label);

	/* first, we need to iterate over the list of arguments,
	* generate code for them, and store the reults of each argument
	* expression in a different register. */

	comp_context_symbol_t* ss = get_symbol(node->children[0]);
	if (ss->purpose != PURPOSE_VECTOR || ss->vector_dimensions < 1) {
		perror("calculate_vector_indexing_address(): given node's sym table ptr "
			"does not have purpose vector! Why? something went wrong here!");
		exit(EXIT_FAILURE);
	}

	/* the registers d hold the values for each of the indexing values. 
	 * */
	char* d[MAX_VECTOR_DIMENSIONS];
	comp_tree_t* c = node->children[1];
	int i = 0;
	while (c) {
		d[i++] = generate_register();
		generate_code(c, d[i]);
		ii = instruction_list_merge(&ii, &c->instr_list);
		c = c->next;
	}

	/* the registers S hold the dimension sizes for each of the dimensions. 
	 * */
	char* S[MAX_VECTOR_DIMENSIONS];
	for (i = ss->vector_dimensions - 1; i >= 0; --i) {
		S[i] = generate_register();
		instruction_list_add(&ii);
		ii->opcode = OP_LOAD_I;
		ii->tgt_op_1 = S[i];
		ii->src_op_1 = int_str(ss->vector_dimension_sizes[i]);
	}

	/* the registers S_accum[i] hold the multiplication of the dimensino sizes
	 * with index greater than i.
	 * For instance,
	 * S_accum[n - 1] = 1. 
	 * S_accum[n - 2] = s[n-1] * s_accum[n-1] = s[n-1] * 1. 
	 * S_accum[n - 3] = s[n-2] * s_accum[n-2] = s[n-2] * s[n-1]. 
	 * And so on. */
	char* S_accum[MAX_VECTOR_DIMENSIONS];

	instruction_list_add(&ii);
	ii->opcode = OP_I_2_I;	
	ii->src_op_1 = d[ss->vector_dimensions - 1];
	ii->tgt_op_1 = regdest_address;

	for (i = ss->vector_dimensions - 2; i >= 0; --i) {
		S_accum[i] = generate_register();
		instruction_list_add(&ii);
		if (i != ss->vector_dimensions - 2) {
			ii->opcode = OP_MULT;
			ii->src_op_1 = S_accum[i + 1];
			ii->src_op_2 = S[i + 1];
			ii->tgt_op_1 = S_accum[i];
		} else {
			ii->opcode = OP_I_2_I;
			ii->src_op_1 = S[i + 1];
			ii->tgt_op_1 = S_accum[i];
		}

		instruction_list_add(&ii);
		char* tmp_mult_reg = generate_register(); /* this register will hold 
												  d[i] * S_accum[i] */
		ii->opcode = OP_MULT;
		ii->tgt_op_1 = tmp_mult_reg;
		ii->src_op_1 = d[i];
		ii->src_op_2 = S_accum[i];

		/* now finally accumulate it to regdest */
		instruction_list_add(&ii);
		ii->opcode = OP_ADD;
		ii->src_op_1 = tmp_mult_reg;
		ii->src_op_2 = regdest_address;
		ii->tgt_op_1 = regdest_address;
	}

	/* now, multiply the relative offset by the data size. */
	int datasize = 1;
	if (ss->type == IKS_BOOL) datasize = 1;
	else if (ss->type == IKS_INT) datasize = 4;
	else if (ss->type == IKS_FLOAT) datasize = 8;
	else if (ss->type == IKS_CHAR) datasize = 1;
	else if (ss->type == IKS_STRING) datasize = 100;
	else {
		perror("Invalid data type when calculating vector indexing.");
		exit(EXIT_FAILURE);
	}

	instruction_list_add(&ii);
	ii->opcode = OP_MULT_I;
	ii->src_op_1 = regdest_address;
	ii->src_op_2 = int_str(datasize);
	ii->tgt_op_1 = regdest_address;

	instruction_list_add(&ii);
	ii->opcode = OP_NOP;
	ii->label = (char*)malloc(256 * sizeof(char));
	sprintf(ii->label, "vector_indexing_%d_END", label_count);
	str_pool_add(ii->label);

	++label_count;
}

void generate_code_literal(comp_tree_t* node, char* regdest)
{
	if(node->sym_table_ptr == NULL)
	{
		printf("Error: no symbol's table pointer exists in literal\n");
		exit(-1);
	}
		
	instruction_list_add(&(node->instr_list));
	node->instr_list->opcode = OP_LOAD_I;

	node->instr_list->src_op_1 = (char*) node->sym_table_ptr->token;

	node->instr_list->tgt_op_1 = regdest;
}

void generate_code_operation_negative(comp_tree_t* node, char* regdest)
{
	// register for value of children 0
	char* r1 = generate_register();	
	
	generate_code(node->children[0],r1);
	
	char *zero = (char*) malloc(2);
	strcpy(zero, "0");
	
	node->instr_list = instruction_list_merge(&node->instr_list, &node->children[0]->instr_list);
	
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_R_SUB_I;
	node->instr_list->src_op_1 = r1;
	node->instr_list->src_op_2 = zero;
	node->instr_list->tgt_op_1 = regdest;
}

void generate_code_operation(comp_tree_t* node, char* regdest, int operation)
{
	// register for value of children 0
	char* r1 = generate_register();	
	// register for value of chldren 1
	char* r2 = generate_register();
	
	// Generate code for first and second expression
	generate_code(node->children[0],r1);
	generate_code(node->children[1],r2);
	
	// Add first expression and second code to this node
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));
	
	instruction_list_add(&(node->instr_list));
	node->instr_list->opcode = operation;
	node->instr_list->src_op_1 = r1;
	node->instr_list->src_op_2 = r2;
	node->instr_list->tgt_op_1 = regdest;
	
}

void generate_code_boolean_operation(comp_tree_t* node, char* regdest, int operation)
{
	char* short_circuit_label;
	char* second_exp_label = generate_label();
	char* next_label = generate_label();

	// register for value of children 0
	char* r1 = generate_register();	
	// register for value of chldren 1
	char* r2 = generate_register();
	
	// Generate code for first and second expression
	generate_code(node->children[0],r1);
	generate_code(node->children[1],r2);
	
	// Add first expression code to this node
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));

	short_circuit_label = generate_label();
	instruction_list_add(&(node->instr_list));
	node->instr_list->opcode = OP_CBR;
	node->instr_list->src_op_1 = r1;

	if(operation == OP_AND)
	{
		node->instr_list->tgt_op_1 = second_exp_label;
		node->instr_list->tgt_op_2 = short_circuit_label;
	}
	else
	{
		node->instr_list->tgt_op_1 = short_circuit_label;
		node->instr_list->tgt_op_2 = second_exp_label;
	}

	// Add second expression label
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = second_exp_label;

	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));
	
	instruction_list_add(&(node->instr_list));
	node->instr_list->opcode = operation;
	node->instr_list->src_op_1 = r1;
	node->instr_list->src_op_2 = r2;
	node->instr_list->tgt_op_1 = regdest;
	
	// In case there was no short circuit, we jump to next instruction set
	// This MUST be added before the short_circuit_label, in case we do not jump directly
	// to the short circuit, else we will copy R1 to REGDEST without need
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_JUMP_I;
	node->instr_list->tgt_op_1 = next_label;		

	// Add short circuit label, in case there is short circuit
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = short_circuit_label;

	// There may be short circuit, so we copy the contents of the short circuited register (R1) to REGDEST
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_I_2_I;
	node->instr_list->src_op_1 = r1;
	node->instr_list->tgt_op_1 = regdest;

	// Code for the next set of instructions
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = next_label;
}

void generate_children_code(comp_tree_t* node, char* regdest)
{
	int i;
	for(i=0; i < node->num_children; i++)
	{
		comp_tree_t *c = node->children[i];
		while(c!=NULL)
		{
			generate_code(c,regdest);
			node->instr_list = instruction_list_merge(&node->instr_list, &c->instr_list);
			c = c->next;
		}
	}
	
}
void generate_code_if_else(comp_tree_t* node)
{
	// Generate code for IF-ELSE condition test
	char* cond_reg = generate_register();
	generate_code(node->children[0], cond_reg);
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));

	// Generate labels for condition cases TRUE and FALSE
	char* case_true_label = generate_label();
	char* case_false_label = generate_label();
	char* next_label = generate_label();

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
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));
	
	// After true code is executed, go to next instruction
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_JUMP_I;
	node->instr_list->tgt_op_1 = next_label;

	// Add condition false case label
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_false_label;

	// Add condition false case code block, if it exists
	if(node->children[2] != NULL)
	{
		generate_code(node->children[2], NULL);
		node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[2]->instr_list));

		// After false case code is executed, go to next instruction
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_JUMP_I;
		node->instr_list->src_op_1 = next_label;
	}

	// Add next instruction label
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = next_label;
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
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));

	// Add instruction list of DO-WHILE condition test
	char* cond_reg = generate_register();
	generate_code(node->children[0], cond_reg);
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));

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
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));

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
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));

	// Add label to case when test is false (which means going to the next instruction)
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = case_false_label;
}

void generate_code_comparison(comp_tree_t* node, char* regdest, int comparison)
{
	// Generate registers for the first and second expression of the comparison
	char* first_exp_reg = generate_register();
	char* secnd_exp_reg = generate_register();

	// Generate code for the first and second expressions
	generate_code(node->children[0], first_exp_reg);
	generate_code(node->children[1], secnd_exp_reg);

	// Add children code to this node
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[0]->instr_list));
	node->instr_list = instruction_list_merge(&node->instr_list, &(node->children[1]->instr_list));

	// Execute the desired comparison
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = comparison;
	node->instr_list->src_op_1 = first_exp_reg;
	node->instr_list->src_op_2 = secnd_exp_reg;
	node->instr_list->tgt_op_1 = regdest;
}
