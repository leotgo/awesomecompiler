#include "cc_context.h"
#include "cc_type.h"
#include "cc_error.h"
#include <stdlib.h>
#include <stdio.h>

comp_context_t* main_context = NULL;
comp_context_t* current_context = NULL;
__attribute__((constructor)) void begin()
{
	main_context = context_push_new();
	current_context = main_context;
}
__attribute__((destructor)) void end()
{
	context_pop();
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
		const char* identifier, int type, int purpose, int vector_dimensions) 
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
	sym->type = type;
	sym->key = (const char*)malloc(sizeof(char) * (1 + strlen(identifier)));
	sym->data_size = 0;
	
	sym->vector_size = vector_dimensions;
	strcpy((char*)sym->key, identifier);

	HASH_ADD_KEYPTR(hh, current_context->symbols_table, sym->key,
		strlen(sym->key), sym);
	
	calculate_symbol_data_size(sym);

	return sym;
}

int calculate_symbol_data_size(comp_context_symbol_t* sym) {
	if (sym == NULL) return 0;
	if (sym->type == IKS_BOOL) sym->data_size = 1;
	else if (sym->type == IKS_INT) sym->data_size = 4;
	else if (sym->type == IKS_FLOAT) sym->data_size = 8;
	else if (sym->type == IKS_CHAR) sym->data_size = 1;
	else if (sym->type == IKS_STRING) {
		sym->data_size = 1;
	}
	if (sym->purpose == PURPOSE_VECTOR)
		sym->data_size *= sym->vector_size;
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
	sym->vector_size = 1;
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

void _context_calc_addr(comp_context_t* ctx) {	
	comp_context_symbol_t* sym, *tmp;
	if (ctx == NULL) return;
	ctx->end_addr = ctx->start_addr;
	HASH_ITER(hh, ctx->symbols_table, sym, tmp) {
		sym->addr = ctx->end_addr;
		ctx->end_addr += sym->data_size;
	}

	if (ctx->next != NULL) {
		ctx->next->start_addr = ctx->start_addr;
		_context_calc_addr(ctx->next);
	}

	if (ctx->children != NULL) {
		ctx->children->start_addr = ctx->end_addr;
		_context_calc_addr(ctx->children);
	}
}

void context_calc_addr() {
	main_context->start_addr = 0;
	main_context->end_addr = 0;
	_context_calc_addr(main_context);
}
