#ifndef __ERROR_H
#define __ERROR_H

#define IKS_SUCCESS 0 //caso n�o houver nenhum tipo de erro

/* Verifica��o de declara��es */
#define IKS_ERROR_UNDECLARED 1 /* identificador n�o declarado */
#define IKS_ERROR_DECLARED 2 /* identificador j� declarado */

/* Uso correto de identificadores */
#define IKS_ERROR_VARIABLE 3 /* identificador deve ser utilizado como 
								vari�vel */
#define IKS_ERROR_VECTOR 4 /* identificador deve ser utilizado como vetor */
#define IKS_ERROR_FUNCTION 5 /* identificador deve ser utilizado como fun��o */

/* Tipos e tamanho de dados */
#define IKS_ERROR_WRONG_TYPE 6 /* tipos incompat�veis */
#define IKS_ERROR_STRING_TO_X 7 /* coer��o imposs�vel do tipo string */
#define IKS_ERROR_CHAR_TO_X 8 /* coer��o imposs�vel do tipo char */

/* Argumentos e par�metros */
#define IKS_ERROR_MISSING_ARGS 9 /* faltam argumentos */
#define IKS_ERROR_EXCESS_ARGS 10 /* sobram argumentos */
#define IKS_ERROR_WRONG_TYPE_ARGS 11 /* argumentos incompat�veis */

/* Verifica��o de tipos em comandos */
#define IKS_ERROR_WRONG_PAR_INPUT 12 /* par�metro n�o � identificador */
#define IKS_ERROR_WRONG_PAR_OUTPUT 13 /* par�metro n�o � literal string ou 
										 express�o */
#define IKS_ERROR_WRONG_PAR_RETURN 14 /* par�metro n�o � express�o compat�vel 
										 com tipo do retorno */

#define IKS_ERROR_FUNCTION_NO_RETURN 15

#endif
