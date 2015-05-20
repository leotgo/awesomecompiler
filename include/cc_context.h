#ifndef __CONTEXT_H
#define __CONTEXT_H

#include "cc_dict.h"
#include "uthash.h"
#include "cc_type.h"

#define PURPOSE_NORMAL 0
#define PURPOSE_VECTOR 1
#define PURPOSE_FUNCTION 2
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
struct comp_dict_item_t;
typedef struct vector_dimension_list vector_dimension_list;
/* the symbols table that will be stored inside the compiler contexts */
typedef struct comp_context_symbol_t {
	const char* key; /* string defining the entry's key (i.e., the name of
					 the identifier */
	int type; /* type as defined by cc_type.h */

	struct type_list* parameters; /* list with all parameters if element is 
								  function. If not, value will be NULL */

	int purpose; /*0 = normal variable; 1 = vector; 2 = function; */

	int data_size;

	int vector_size; /* if purpose == vector, then this holds the size of 
					 the vector. otherwise, it is equal to 0. */

	int addr; /* the address defined by the variable in the
						  current activation registry. */

	UT_hash_handle hh; /* hash handle. */

} comp_context_symbol_t;

typedef comp_context_symbol_t* comp_context_symbols_table_t;

/* deallocates all the resources for a context table. */
void context_symbol_free(comp_context_symbol_t* s);

typedef struct comp_context_t {
	int start_addr; /* the address where the first declared variable is
						 be placed in this context. */
	int end_addr; /* one past the last address that is used by this context. 
					 */
	comp_context_symbols_table_t symbols_table;
	struct comp_context_t* parent;
	struct comp_context_t* children;
	struct comp_context_t* next; /* the next sibling in the parent's 
								 children list */
} comp_context_t;

/* deallocates the memory for the given context, and all of its children. */
void context_free(comp_context_t* context);

/* creates a new context, adds it to the current context's children, and sets
 * it as the current context.
 * if there is no current context yet, it is set as the main (global) context. */
comp_context_t* context_push_new();

/* sets the current context to be the parent of the current context (i.e., goes
 * back one level on the context stack). */
void context_pop();

/* searches 'identifier' in the given context. if it doesn't exist in the
 * given context's symbols table, returns NULL. otherwise, returns a pointer 
 * to the symbol in the table corresponding to 'identifier'. */
comp_context_symbol_t* context_find_identifier(comp_context_t* context, 
	const char* identifier);

/* searches 'identifier' in the given context, and then, recursively,
 * in its parent contexts. if it is not found in any level above, returns NULL.
 * otherwise, returns a pointer to the first symbol corresponding to 
 * 'identifier' that is found. */
comp_context_symbol_t* context_find_identifier_multilevel(
	comp_context_t* context, const char* identifier);

/* adds the identifier with type 'type' to the current context. if it already
 * exists in the current context, the program exits with code 
 * IKS_ERROR_DECLARED. otherwise, adds the identifier to the current context. 
 * */
comp_context_symbol_t* context_add_identifier_to_current(
	const char* identifier, int type, int purpose, int vector_dimensions);

comp_context_symbol_t* context_add_function_to_current(
	const char* identifier, int type, struct comp_dict_item_t* parameters);

int calculate_symbol_data_size(comp_context_symbol_t* sym);

/* 
 * calculates the addresses of all variables declared in the program, within
 * all the contexts.
 * 
 * this function searches over main_context and its children and sets the fields
 * 'start_addr', 'end_addr' in context_t and 'addr' in the context_symbol_t.
 * 
 * the main context starts at address 0. all global variables' addresses are
 * declared at this level. 
 * 
 * example:
 * 
 * int x; -> address 0, size = 4
 * string z = "alex zoch gliesch" -> address 4, size = 18 (17 characters + \0)
 * 
 * void main() {
 *		int x = 1000; -> address 26, size 4;
 *		while (x != 0) {
 *			int m = -1; -> address 30, size 4;
 *			x = x + m;
 *		}
 *		while (x < 1000) {
 *			int n = 5; -> address 30, size = 4;
 *			x = x + n;
 *		}
 * }
 * 
 * int y; -> address 22, size 4
 * 
 * */
void context_calc_addr();

/* the main context is where global variables and the all the function 
 * declarations are defined. */
extern comp_context_t* main_context;

/* a pointer to the current context */
extern comp_context_t* current_context;

#endif
