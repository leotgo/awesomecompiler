#include "cc_misc.h"
#include "cc_dict.h"

extern int lineCounter; /* lineCounter is declared in scanner.c, 
						which is generated by flex. */

extern comp_dict_t symbols_table; /* the compiler's symbols table, declared
								  in cc_dict.c */

int getLineNumber(void) {
	return lineCounter;
}

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem); //altere para que apareça a linha
}

void main_init (int argc, char **argv)
{
  //implemente esta função com rotinas de inicialização, se necessário
}

void main_finalize(void) {
	symbols_table_finalize(&symbols_table);
}

int recognize_token(const char* token_text, int token_id) {
	/* first, add the token to the symbols table. */
	symbols_table_add(token_text, getLineNumber(), &symbols_table);

	/* then, return the token's identifier. */
	return token_id;
}
