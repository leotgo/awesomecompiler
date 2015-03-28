#ifndef __MISC_H
#define __MISC_H
#include <stdio.h>

int getLineNumber (void);

void yyerror (char const *mensagem);

void main_init (int argc, char **argv);

void main_finalize (void);

/* receives a token, and, if it needs to be added to the symbols table, add it.
 * return its token id at the end. */
int recognize_token(const char* token_text, int token_id);

/* given a lexeme and it's type, computes the key that will be used for that
* lexeme in the symbols table, and returns a pointer to it. note: the string
* must be then deallocated later! */
char* compute_symbols_table_key(const char* token_text, int token_id);

/* helper function that takes a string, and returns a copy of it without the
* first and last element. */
char* remove_quotes(const char* token_text);

#endif
