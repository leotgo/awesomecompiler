/*
  cc_ast.h

  Este arquivo contém as constantes para os tipos dos nós da AST.
*/
#ifndef __CC_AST_H
#define __CC_AST_H
#define AST_PROGRAMA             0
#define AST_FUNCAO               1
#define AST_IF_ELSE              2
#define AST_DO_WHILE             3
#define AST_WHILE_DO             4
#define AST_INPUT                5
#define AST_OUTPUT               6
#define AST_ATRIBUICAO           7
#define AST_RETURN               8
#define AST_BLOCO                9
#define AST_IDENTIFICADOR       10
#define AST_LITERAL             11
#define AST_ARIM_SOMA           12
#define AST_ARIM_SUBTRACAO      13
#define AST_ARIM_MULTIPLICACAO  14
#define AST_ARIM_DIVISAO        15
#define AST_ARIM_INVERSAO       16 // - (operador unário -)
#define AST_LOGICO_E            17 // &&
#define AST_LOGICO_OU           18 // ||
#define AST_LOGICO_COMP_DIF     19 // !=
#define AST_LOGICO_COMP_IGUAL   20 // ==
#define AST_LOGICO_COMP_LE      21 // <=
#define AST_LOGICO_COMP_GE      22 // >=
#define AST_LOGICO_COMP_L       23 // <
#define AST_LOGICO_COMP_G       24 // >
#define AST_LOGICO_COMP_NEGACAO 25 // !
#define AST_VETOR_INDEXADO      26 // para var[exp] quando o índice exp é acessado no vetor var
#define AST_CHAMADA_DE_FUNCAO   27
#endif
