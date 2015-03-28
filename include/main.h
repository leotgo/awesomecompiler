/*
  main.h

  Cabeçalho principal do analisador sintático
*/
#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>
#include "cc_dict.h"
#include "cc_list.h"
#include "cc_tree.h"
#include "parser.h"
#include "cc_misc.h"
#include "cc_gv.h"

extern int yylex(void);
/*
  Constantes a serem utilizadas como valor de retorno no caso de
  sucesso (SINTATICA_SUCESSO) e erro (SINTATICA_ERRO) do analisador
  sintático.
*/
#define SINTATICA_SUCESSO 0
#define SINTATICA_ERRO    1

/*
  Constantes a serem utilizadas para diferenciar os lexemas que estão
  registrados na tabela de símbolos.
*/
enum { 
	SIMBOLO_LITERAL_INT = 1,
	SIMBOLO_LITERAL_FLOAT,
	SIMBOLO_LITERAL_CHAR,
	SIMBOLO_LITERAL_STRING,
	SIMBOLO_LITERAL_BOOL,
	SIMBOLO_IDENTIFICADOR,
	SIMBOLO_ERRO
};

#endif
