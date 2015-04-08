#include "cc_dict.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

void free_comp_dict_item_t(comp_dict_item_t* item) {
	if (item != NULL) {
		if (item->token != NULL) 
			free((void*)item->token);					
		if (item->value != NULL)
			free((void*)item->value);
		free(item);
	}
}

/* The global symbols table defined in the program. */
comp_dict_t symbols_table = NULL;

comp_dict_item_t* symbols_table_add(const char* key, int line_number,
	int token_type, const char* token_value, comp_dict_t* table) {

	comp_dict_item_t* item = symbols_table_find(key, table);

	if (item != NULL) {
		/* an item with the same key is already in the symbols table.
		 * we have to update the line where it appeared and the value. */
		item->line_where_it_last_appeared = line_number;

		if (item->value != NULL) 
			free(item->value);

		item->value = interpret_token_value(token_value, token_type, 
			line_number);

		/* note that we don't have to change the token_type or worry about
		 * the value's type, since, if the token_type were different,
		 * the key values would be different anyway, since the token_type is
		 * embedded in the key. */
		return item;
	}

	item = (comp_dict_item_t*) malloc(sizeof(comp_dict_item_t));

	item->token = (const char*) malloc((strlen(key) + 1) * sizeof(char));
	strcpy((char*)item->token, key);	
	item->line_where_it_last_appeared = line_number;
	item->value = interpret_token_value(token_value, token_type, line_number);
	item->token_type = token_type;

	HASH_ADD_KEYPTR(hh, *table, item->token, strlen(item->token), item);

	return item;
}

comp_dict_item_t* symbols_table_find(const char* key, comp_dict_t* table) {
	comp_dict_item_t* item = NULL;
	HASH_FIND_STR(*table, key, item);
	return item;
}

int symbols_table_remove(const char* key, comp_dict_t* table) {
	comp_dict_item_t* item = symbols_table_find(key, table);
	if (item != NULL) {
		HASH_DEL(*table, item);
		free_comp_dict_item_t(item);		
		return 1;
	} else return 0;
}

void symbols_table_finalize(comp_dict_t* table) {
	if (table == NULL) return;
	comp_dict_item_t* item, *tmp;
	HASH_ITER(hh, *table, item, tmp) {
		HASH_DEL(*table, item);
		free_comp_dict_item_t(item);
	}
}

int symbols_table_count(comp_dict_t* table) {
	return HASH_COUNT(*table);
}

void* interpret_token_value(const char* text, int token_type, int line_number) {
	void* value = NULL;
	if (token_type == SIMBOLO_LITERAL_INT) {
		value = malloc(sizeof(int));
		*((int*)value) = strtol(text, NULL, 10);
		if (errno == ERANGE) {
			/* int value is off integer limits, defaults to 2^31 - 1. */
			printf("warning: line %d: integer value %s is off limits. "
				"defaulting to %d\n", line_number, text, *((int*)value));
		}
	} else if (token_type == SIMBOLO_LITERAL_CHAR) {
		value = malloc(sizeof(char));
		*((char*)value) = text[0];
	} else if (token_type == SIMBOLO_LITERAL_BOOL) {
		value = malloc(sizeof(int));		
		*((int*)value) = strcmp(text, "false"); /* strcmp returns 0 if strings 
										are equal, and different than 0 
										otherwise. so, if it's false, we'll get 
										0 anyway :) */
	} else if (token_type == SIMBOLO_LITERAL_FLOAT) {
		value = malloc(sizeof(float));
		*((float*)value) = strtof(text, NULL);
		if (errno == ERANGE) {
			/* float value overflow. */
			printf("warning: line %d: float value %s is off limits. "
				"defaulting to %.2f\n", line_number, text, *((float*)value));
		}
	} else if (token_type == SIMBOLO_LITERAL_STRING) {
		value = malloc((strlen(text) + 1) * sizeof(char));
		strcpy(value, text);
	} else if (token_type == SIMBOLO_IDENTIFICADOR) {
		/* if the token is an identifier, it has no value associated to it
		 * for now. */
		value = NULL;
	} else {
		/* error. */
		printf("invalid token type (%d) passed to interpret_token_value().\n",
			token_type);
		value = NULL;
	}
	return value;
}
