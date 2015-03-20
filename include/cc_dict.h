#ifndef __DICT_H
#define __DICT_H

/* for the symbol table, we used the library 'uthash', which is available 
 * for free at http://troydhanson.github.io/uthash/ */
#include "uthash.h" 

/* The structure defining the elements that the symbols table will store, as
 * defined by the requirements. */
typedef struct comp_dict_item_t {
	const char* token; /* a 0-terminated string representing the matched token. 
					   */

	int line_where_it_last_appeared; /* line where the specific token appeard
									 last */

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
 * already exists in the table, returns NULL. */
comp_dict_item_t* symbols_table_add(const char* key, int line, 
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

#endif