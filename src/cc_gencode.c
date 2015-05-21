#include "cc_gencode.h"
#include "cc_type.h"
#include "cc_register.h"
#include "cc_label.h"
#include "cc_iloc.h"

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
			/* load operando 1, registrador temporario r1
			 * load operando 2, registrador temporario r2
			 * sum r1 r2
			 * coloca a sum no registrador DE RETORNO r3 (passado como parametro
			 * pra generate_code()) 
			 * (x * 1) + (y * 3)  
			 */
			break;
		case AST_ARIM_SUBTRACAO:
			break;
		case AST_ARIM_MULTIPLICACAO:
			break;
		case AST_ARIM_DIVISAO:
			break;
		case AST_ARIM_INVERSAO:
			break;
		case AST_LOGICO_E:			
			break;
		case AST_LOGICO_OU:
			break;
		/* ****************************************** */
		
		
		/* Alex ************************************* */
		case AST_IDENTIFICADOR:
			/* 
			 * não tem código, mas aqui tem que ver o ponteiro
			 * pro simbolo no contexto e setar node->addr adequadamente. 
			 * */
			node->addr = get_symbol(node)->addr;

			/* if the user specified a destination registrer, we make a 
			 * LOAD of this identifier's value to rdest. */
			if (regdest) {				
				instruction_list_add(&node->instr_list);
				node->instr_list->opcode = OP_LOAD_A_I;
				node->instr_list->tgt_reg_1 = regdest;
				if (node->context == main_context) { /* global variable */
					node->instr_list->src_reg_1 = reg_fp();
				} else {
					node->instr_list->src_reg_1 = reg_arp();
				}				

				//node->instr_list->src_reg_2 = 
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


			char* reg0 = generate_register();
			/* generate code for the left side of the attribution. */
			generate_code(node->children[0], reg0);

			char* reg1 = generate_register();

			/* generate code for the right side of the attrib.*/
			generate_code(node->children[1], reg1);

			/* addr of the variable that will be assig. */
			int dest_addr = get_symbol(node->children[0])->addr; 

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
