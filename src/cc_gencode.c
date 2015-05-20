#include "cc_gencode.h"


// The main code generation function. Called over the AST tree root, after it 
// is created.

void generate_code(comp_tree_t* node)
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
			break;
		case AST_ATRIBUICAO:
			break;
		case AST_VETOR_INDEXADO:
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
