#include "cc_context.h"
#include "cc_type.h"
#include "cc_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int global_sequence_number = 0;
comp_context_t* main_context = NULL;
comp_context_t* current_context = NULL;
__attribute__((constructor)) void begin()
{
	main_context = context_push_new();
	current_context = main_context;
}
__attribute__((destructor)) void end()
{
	//context_pop();
}

void context_free(comp_context_t* context) {
	if (context->children != NULL) {
		comp_context_t *b, *c;
		for (c = context->children; c != NULL; ) {
			b = c;
			c = c->next;
			context_free(b);
		}
	}
	/* finalize the symbols table */
	if (context->symbols_table != NULL) {
		comp_context_symbol_t* item, *tmp;
		HASH_ITER(hh, context->symbols_table, item, tmp) {
			HASH_DEL(context->symbols_table, item);
			context_symbol_free(item);
		}
	}
	free(context);
}

comp_context_t* context_push_new() {
	//yyerror("context push!");
	comp_context_t* parent = current_context;

	current_context = (comp_context_t*)malloc(sizeof(comp_context_t));
	current_context->symbols_table = NULL;
	current_context->parent = NULL;
	current_context->children = NULL;
	current_context->next = NULL;


	/* set the parent */
	current_context->parent = parent;

	if (parent != NULL) {
		/* set the current context in its parents children. */
		comp_context_t** c = NULL;
		for (c = &parent->children; *c != NULL; c = &((*c)->next));
		*c = current_context;
	}

	if (main_context == NULL)
		main_context = current_context;		
	return current_context;
}

void context_pop() {
	if (current_context != NULL) {
		if (current_context->parent != NULL) {
			//yyerror("context pop!");
			current_context = current_context->parent;
		} else {
			/* error: current context has no parent */
			//context_free(current_context);
			
			/* we should never pop the main context; instead, call 
			 * context_free() on it when we are finalizing all data structures. 
			 * */

			current_context = NULL;
			yyerror("Context has no parent!\n"
				"We are popping the main context, which should never happen. "
				"We can't pop the main context yet, because we will need them "
				"when we go through the grammar one more time.\n");
		}
	} else {
		/* error: popping NULL context */
	}
}

comp_context_symbol_t* context_add_identifier_to_current(
		const char* identifier, int type, int purpose, int vector_dimensions,
		int* vector_dimension_sizes) 
{
	if (current_context == NULL )
	{
		current_context = main_context;
	}
	if ( identifier == NULL || type == IKS_INVALID) 
	{
		/* error */
		return NULL;
	}
	
	if (context_find_identifier(current_context, identifier) != NULL) 
	{
		
		yyerror("Error: Variable or function already declared in scope");
		/* error: variable was already declared in this scope */
		exit(IKS_ERROR_DECLARED);
	}
	
	comp_context_symbol_t* sym = (comp_context_symbol_t*)
		malloc(sizeof(comp_context_symbol_t));
	sym->parameters = NULL;
	sym->purpose = purpose;
	sym->type = type_convert(type);
	sym->key = (const char*)malloc(sizeof(char) * (1 + strlen(identifier)));
	sym->data_size = 0;
	sym->function_code_label = NULL;
	sym->sequence_number = ++global_sequence_number;
	sym->act_frame = NULL;
	sym->vector_dimensions = vector_dimensions;
	memset(sym->vector_dimension_sizes, -1, 
		sizeof(sym->vector_dimension_sizes));
	if (vector_dimension_sizes != NULL && vector_dimensions > 0) {
		memcpy(sym->vector_dimension_sizes, vector_dimension_sizes,
			sizeof(int) * (vector_dimensions + 1));
	}
	strcpy((char*)sym->key, identifier);

	HASH_ADD_KEYPTR(hh, current_context->symbols_table, sym->key,
		strlen(sym->key), sym);
	
	calculate_symbol_data_size(sym);

	return sym;
}

int calculate_symbol_data_size(comp_context_symbol_t* sym) {
	if (sym == NULL) return 0;
	sym->data_size = type_data_size(sym->type);
	if (sym->purpose == PURPOSE_VECTOR) {
		int d;
		for (d = 0; d < sym->vector_dimensions; ++d) {
			if (sym->vector_dimension_sizes[d] != -1)
				sym->data_size *= sym->vector_dimension_sizes[d];
		}
	}
	return sym->data_size; 
}

comp_context_symbol_t* context_add_function_to_current(
		const char* identifier, int type, comp_dict_item_t* parameters) 
{
	if (current_context == NULL )
	{
		printf("CURRENT IS NULL!\n");
		current_context = main_context;	
	}
	if ( identifier == NULL || type == IKS_INVALID) 
	{
		/* error */
		return NULL;
	}
	
	if (context_find_identifier(current_context, identifier) != NULL) 
	{
		
		yyerror("Error: Function already declared in scope");
		/* error: variable was already declared in this scope */
		exit(IKS_ERROR_DECLARED);
	}
	
	comp_context_symbol_t* sym = (comp_context_symbol_t*)
		malloc(sizeof(comp_context_symbol_t));
	if(parameters != NULL)
	{
		sym->parameters = parameters->params_list;
	}
	else
	{
		sym->parameters = NULL;
	}
	sym->purpose = PURPOSE_FUNCTION;
	sym->type = type;
	sym->key = (const char*)malloc(sizeof(char) * (1 + strlen(identifier)));
	sym->data_size = 0;
	sym->vector_dimensions = 0;
	sym->function_code_label = NULL;
	sym->sequence_number = ++global_sequence_number;

	strcpy((char*)sym->key, identifier);


	HASH_ADD_KEYPTR(hh, current_context->symbols_table, sym->key,
		strlen(sym->key), sym);
	
	return sym;
}

void context_symbol_free(comp_context_symbol_t* s) 
{
	if (s != NULL) 
	{
		if (s->key != NULL)
			free((char*)s->key);
		if (s->parameters != NULL)
			type_list_free(s->parameters);
		free(s);
	}
}

comp_context_symbol_t* context_find_identifier(comp_context_t* context, 
		const char* identifier) 
{

	comp_context_symbol_t* item = NULL;
	HASH_FIND_STR(context->symbols_table, identifier, item);
	return item;
}

comp_context_symbol_t* context_find_identifier_multilevel(
		comp_context_t* context, const char* identifier) 
{

	if (context == NULL) return NULL;	
	comp_context_t* c = context;
	while (c != NULL) {
		comp_context_symbol_t* s = context_find_identifier(c, identifier);
		if (s != NULL) return s;
		c = c->parent;
	}
	return NULL;
}

int compare_symbols_in_context(const void* sym_a, const void* sym_b) {
	comp_context_symbol_t* a = *(comp_context_symbol_t**)sym_a;
	comp_context_symbol_t* b = *(comp_context_symbol_t**)sym_b;
	return a->sequence_number > b->sequence_number;
}

void _context_calc_addr(comp_context_t* ctx) {	
	comp_context_symbol_t* sym, *tmp;
	if (ctx == NULL) return;
	ctx->end_addr = ctx->start_addr;

	comp_context_symbol_t* symbols_in_context[500];
	int num_symbols_in_context = 0;

	HASH_ITER(hh, ctx->symbols_table, sym, tmp) {
		symbols_in_context[num_symbols_in_context] = sym;
		++num_symbols_in_context;
	}

	if (num_symbols_in_context > 0) {
		qsort(symbols_in_context, num_symbols_in_context,
			sizeof(comp_context_symbol_t*), compare_symbols_in_context);

		int i;
		for (i = 0; i < num_symbols_in_context; ++i) {
			if (i > 0) {
				//printf("i seq num: %d\ni-1 seq num: %d\n",
				//	symbols_in_context[i]->sequence_number,
				//	symbols_in_context[i - 1]->sequence_number);
				assert(symbols_in_context[i]->sequence_number
				> symbols_in_context[i - 1]->sequence_number);
			}
			symbols_in_context[i]->addr = ctx->end_addr;
			ctx->end_addr += symbols_in_context[i]->data_size;
		}
	}

	if (ctx->next != NULL) {
		ctx->next->start_addr = 0;
		_context_calc_addr(ctx->next);
	}

	if (ctx->children != NULL) {
		ctx->children->start_addr = 0;
		_context_calc_addr(ctx->children);
	}
}

void context_calc_addr() {
	main_context->start_addr = 0;
	main_context->end_addr = 0;
	_context_calc_addr(main_context);
}
