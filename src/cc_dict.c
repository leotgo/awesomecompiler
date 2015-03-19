#include "cc_dict.h"
#include <stdlib.h>
#include <string.h>

void free_comp_dict_item_t(comp_dict_item_t* item) {
	if (item != NULL) {
		if (item->token != NULL) 
			free(item->token);
		free(item);
	}
}

/* The global symbols table defined in the program. */
comp_dict_t symbols_table = NULL;

comp_dict_item_t* symbols_table_add(const char* key, int line, 
	comp_dict_t* table) {

	comp_dict_item_t* item = symbols_table_find(key, table);

	if (item != NULL) 
		return NULL;

	item = (comp_dict_item_t*)malloc(sizeof(comp_dict_item_t));
	item->token = (char*)malloc(strlen(key) * sizeof(char));
	strcpy(item->token, key);
	item->line_where_it_last_appeared = line;
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
