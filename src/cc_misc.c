#include "cc_misc.h"
#include "cc_dict.h"
#include "cc_tree.h"
#include "parser.h"
#include "main.h"
#include <assert.h>

extern int lineCounter; /* lineCounter is declared in scanner.c, 
						which is generated by flex. */

extern comp_dict_t symbols_table; /* the compiler's symbols table, declared
								  in cc_dict.c */

extern YYSTYPE yylval;
extern char* yytext;

int getLineNumber(void) {
	return lineCounter;
}

void yyerror (char const *mensagem) {
	int line = getLineNumber();
  	fprintf (stderr, "%s in line %d \n", mensagem, line); 
}

void main_init (int argc, char **argv) {

}

void main_finalize(void) {
	/* free data structures */
	symbols_table_finalize(&symbols_table);	
	free_tree_node(global_syntax_tree);
}

int recognize_token(int token_id) {
	const char* token_text = yytext;
	yylval.valor_simbolo_lexico = NULL;

	/* add the lexeme to symbols table only if it is a literal or an
	 * identificator. */
	if (token_id == TK_IDENTIFICADOR || token_id == TK_LIT_FALSE
		|| token_id == TK_LIT_TRUE || token_id == TK_LIT_INT
		|| token_id == TK_LIT_FLOAT || token_id == TK_LIT_CHAR
		|| token_id == TK_LIT_STRING) {

		/* defined in main.h, it's used to diferentiate the lexemes in the
		* symbols table, by concatenating it to the token text. */
		int token_type_id;
		switch (token_id) {
		case TK_IDENTIFICADOR:
			token_type_id = SIMBOLO_IDENTIFICADOR;
			break;
		case TK_LIT_TRUE:
		case TK_LIT_FALSE:
			token_type_id = SIMBOLO_LITERAL_BOOL;
			break;
		case TK_LIT_INT:
			token_type_id = SIMBOLO_LITERAL_INT;
			break;
		case TK_LIT_FLOAT:
			token_type_id = SIMBOLO_LITERAL_FLOAT;
			break;
		case TK_LIT_CHAR:
			token_type_id = SIMBOLO_LITERAL_CHAR;
			break;
		case TK_LIT_STRING:
			token_type_id = SIMBOLO_LITERAL_STRING;
			break;
		default:
			printf("Invalid token id (%d) given to "
				"compute_symbols_table_key()!\n", token_id);
		}

		/* remove double and single quotes if token is a string or
		* char literal */
		if (token_type_id == SIMBOLO_LITERAL_STRING ||
			token_type_id == SIMBOLO_LITERAL_CHAR) {
			token_text = remove_quotes(token_text);
		}

		/* get the key that refers to that lexeme: */
		char* symbols_table_key = compute_symbols_table_key(token_text, 
			token_type_id);

		/* add the lexeme to the symbols table. */
		yylval.valor_simbolo_lexico = symbols_table_add(symbols_table_key, 
			getLineNumber(), token_type_id, token_text, &symbols_table);

		free((void*)symbols_table_key);

		/* deallocate token_text here, because we created a new string when we
		* removed quotes.*/
		if (token_type_id == SIMBOLO_LITERAL_STRING ||
			token_type_id == SIMBOLO_LITERAL_CHAR)
			free((void*)token_text);
	}

	/* then, return the token's identifier. */
	return token_id;
}

char* remove_quotes(const char* token_text) {
	char* without_quotes = (char*)
		malloc((strlen(token_text) + 1) * sizeof(char));
	strcpy(without_quotes, token_text + 1);
	without_quotes[strlen(without_quotes) - 1] = '\0';
	return without_quotes;
}

char* compute_symbols_table_key(const char* token_text, int token_type_id) {

	/* here, we safely assume that the token_type_id's number will never 
	 * have more more than 20 characters, which is the maximum length of 
	 * a 64-bit integer. */
	char* symbols_table_key = (char*)
		malloc((24 + strlen(token_text)) * sizeof(char));

	/* use something starting with a number and with invalid characters for
	 * an identifier ('$$') so no conflicts arise */
	sprintf(symbols_table_key, "%d $$ %s", token_type_id, token_text);

	return symbols_table_key;
}
