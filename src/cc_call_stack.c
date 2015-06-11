#include "cc_call_stack.h"



void activation_frame_marshall(activation_frame* frame, comp_tree_t* node)
{
		
	comp_context_symbol_t* sym = get_symbol(node->children[0]);
	assert(sym);
	if (sym->function_code_label == NULL) {
		perror("Error: calling function that has not been declared.");
	}	
	
	sym->act_frame = frame;
	
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->comment = str_pool_lit("Going to call %s() now.",
		sym->key);

	/* store return address */
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_STORE_A_I;
	node->instr_list->src_op_1 = frame->return_address;
	node->instr_list->tgt_op_1 = frame->dynamic_link;
	node->instr_list->tgt_op_2 = int_str(ARP_RETURN_ADDR_DISPLACEMENT);
	node->instr_list->comment = str_pool_lit("Save return address (%s).", 
		frame->return_address);

	/* store old sp */
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_STORE_A_I;
	node->instr_list->src_op_1 = frame->dynamic_link;
	node->instr_list->tgt_op_1 = frame->dynamic_link;
	node->instr_list->tgt_op_2 = int_str(ARP_SP_DISPLACEMENT);
	node->instr_list->comment = str_pool_lit("Save sp.");

	/* store old fp */
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_STORE_A_I;
	node->instr_list->src_op_1 = frame->static_link;
	node->instr_list->tgt_op_1 = frame->dynamic_link;
	node->instr_list->tgt_op_2 = int_str(ARP_FP_DISPLACEMENT);
	node->instr_list->comment = str_pool_lit("Save fp.");

	/* -------------------------------------------------------- 
	 * start load arguments 
	 * ----------------------------------------------------------
	 * */
	int new_sp_pos = ARP_RETURN_VALUE_DISPLACEMENT + type_data_size(sym->type);
	char* param_reg = generate_register();
	int param_num = 1;
	comp_tree_t* cc = node->children[1];
	while (cc) 
	{		
		generate_code(cc, param_reg);
		node->instr_list = instruction_list_merge(&node->instr_list, &cc->instr_list);
		/* result will be in param_reg. store that in new_sp_pos */
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_STORE_A_I;
		node->instr_list->src_op_1 = param_reg;
		node->instr_list->tgt_op_1 = frame->dynamic_link;
		node->instr_list->tgt_op_2 = int_str(new_sp_pos);
		node->instr_list->comment = str_pool_lit(
			"Adding argument %d of %s() to activation registry.", param_num++, 
			sym->key);
		new_sp_pos += type_data_size(cc->induced_type_by_coercion);
		cc = cc->next;
	}

	/* --------------------------------------------------------
	* end load arguments
	* ----------------------------------------------------------
	* */

	/* jump to func */

	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_JUMP_I;
	node->instr_list->tgt_op_1 = sym->function_code_label;
	node->instr_list->comment = str_pool_lit("Jump to %s's implementation.", 
		sym->key);
	
	/* create an instruction so we know where to come back */
	instruction_list_add(&node->instr_list);
	node->instr_list->opcode = OP_NOP;
	node->instr_list->label = frame->return_address;
	node->instr_list->comment = str_pool_lit("Return from function call.");

	if (frame->returned_value) 
	{
		assert(type_convert(sym->type) != IKS_INVALID);
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_LOAD_A_I;
		node->instr_list->src_op_1 = frame->dynamic_link;
		node->instr_list->src_op_2 = int_str(ARP_RETURN_VALUE_DISPLACEMENT);
		node->instr_list->tgt_op_1 = frame->returned_value;
		node->instr_list->comment = str_pool_lit(
			"Putting %s's return value in %s.", sym->key, frame->returned_value);
	}
}

void activation_frame_unmarshall( comp_tree_t* node, comp_context_symbol_t* sym, char* regdest, int is_main_func)
{
		if (!is_main_func) {

		/* 'regdest' will have the return value of the function.
		*
		* so now we'll have to put the return value in the memory position where
		* we must store the return, that is, fp + epilogue_size - 4.
		* */
		if (type_data_size(sym->type) > 0) {
			instruction_list_add(&node->instr_list);
			node->instr_list->opcode = OP_STORE_A_I;
			node->instr_list->src_op_1 = regdest;
			node->instr_list->tgt_op_1 = reg_fp();
			node->instr_list->tgt_op_2 = int_str(ARP_RETURN_VALUE_DISPLACEMENT);
			node->instr_list->comment = str_pool_lit(
				"Put return value (which is in %s) in fp + %d, which will be "
				"used by caller.", regdest, ARP_RETURN_VALUE_DISPLACEMENT);
		}

		/* restore FP and SP, and jump back to the place that called the function.
		 * */
	char* jump_back_address = generate_register();
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_LOAD_A_I;
		node->instr_list->src_op_1 = reg_fp();
		node->instr_list->src_op_2 = int_str(ARP_RETURN_ADDR_DISPLACEMENT);
		node->instr_list->tgt_op_1 = jump_back_address;
		node->instr_list->comment = str_pool_lit("Get return address (which is in fp + %d).",
			ARP_RETURN_ADDR_DISPLACEMENT);

		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_LOAD_A_I;
		node->instr_list->src_op_1 = reg_fp();
		node->instr_list->src_op_2 = int_str(ARP_SP_DISPLACEMENT);
		node->instr_list->tgt_op_1 = reg_sp();
		node->instr_list->comment = str_pool_lit("Restore old sp (which is in fp + %d).",
			ARP_SP_DISPLACEMENT);

		char* old_fp_addr = generate_register();
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_LOAD_A_I;
		node->instr_list->src_op_1 = reg_fp();
		node->instr_list->src_op_2 = int_str(ARP_FP_DISPLACEMENT);
		node->instr_list->tgt_op_1 = reg_fp();
		node->instr_list->comment = str_pool_lit("Restore old fp (which is in fp + %d).",
			ARP_FP_DISPLACEMENT);



		/* jump back*/
		instruction_list_add(&node->instr_list);
		node->instr_list->opcode = OP_JUMP;
		node->instr_list->tgt_op_1 = jump_back_address;
		node->instr_list->comment = str_pool_lit(
			"Jump back to the place that called %s().", sym->key);

		pop_fp_data_displacement();
	}
}
