#ifndef __DICT_H
#define __DICT_H

/* for the symbol table, we used the library 'uthash', which is available 
 * for free at http://troydhanson.github.io/uthash/ */
#include "uthash.h" 
#include "cc_type.h"
typedef struct type_list type_list;
typedef struct vector_dimension_list vector_dimension_list;

/* The structure defining the elements that the symbols table will store, as
 * defined by the requirements. */
typedef struct comp_dict_item_t {
	const char* token; /* a 0-terminated string representing the matched token. */

	const char* key; /* the key used for this item in the symbols table. */

	int line_where_it_last_appeared; /* line where the specific token appeard
									 last */

	int token_type; /* the type of the token */

	void* value; /* the value of the token; the type will depend on what
				 token_type is:
				 SIMBOLO_LITERAL_INT - int
				 SIMBOLO_LITERAL_FLOAT - float
				 SIMBOLO_LITERAL_CHAR - char
				 SIMBOLO_LITERAL_STRING - char*
				 SIMBOLO_LITERAL_BOOL - int (0 or 1)
				 SIMBOLO_IDENTIFICADOR - the pointer will be NULL				 
				 */

	type_list* params_list; // has function parameters
	
	//int vector_dimensions;

	UT_hash_handle hh; /* the handle that must be used by the item types of the
					   hash table, defined by 'uthash'*/
} comp_dict_item_t;

/* the hash-table library we used defines that the table type must be a 
 * pointer to the element it stores. */
typedef comp_dict_item_t* comp_dict_t; 

/* deallocates all the resources necessary for a comp_dict_item_t. */
void free_comp_dict_item_t(comp_dict_item_t* item);

/* adds an item to the given symbols table, with key 'key', 
 * and returns a pointer to that item. if something went wront, or the item 
 * already exists in the table, returns NULL. 
 * 
 * parameters:
 * key - the key 
 * line - line where it last appeared
 * token_type - the type of token. the types are defined in main.h
 * token_value - the token value, which will be converted to the appropriate
 *				 type
 * table - the hash table.
 * */
comp_dict_item_t* symbols_table_add(
	const char* key, 
	int line_number, 
	int token_type, 
	const char* token_value, 
	comp_dict_t* table);

/* searches for the symbol with key 'key' in the table. if the item doesn't 
 * exist, returns NULL. */
comp_dict_item_t* symbols_table_find(const char* key, comp_dict_t* table);

/* removes the item with key 'key' from the given table. returns 1 if 
 * successfull, and 0 otherwise. */
int symbols_table_remove(const char* key, comp_dict_t* table);

/* returns the number of items in the symbols table. */
int symbols_table_count(comp_dict_t* table);

/* delete all the elements in the table, deallocating their memory, and
 * finalizes the table. */
void symbols_table_finalize(comp_dict_t* table);

/* given a text and the token type, computes the value corresponding to that
 * text, puts it in an allocated void*, and returns it. 
 * see comp_dict_item_t::value for more info. */
void* interpret_token_value(const char* text, int token_type, int line_number);

extern comp_dict_t global_symbols_table;

#endif
