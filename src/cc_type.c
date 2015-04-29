#include "cc_type.h"
#include "cc_context.h"

// Adds a type to the specified type list
type_list* type_list_Add(type_list* list);

// Compares two type lists and returns true if they are identical
// (same elements in same order)
bool type_list_Compare(type_list* list_a, type_list* list_b);

bool typeCheck(comp_tree_t* ast)
{
	// Recursive DFS to verify typing for basic expressions
	// Iterate for each child and typecheck	them
	for(int i = 0; i < ast->num_children; i++)
	{
		typecheck(ast->children[i]);
	}
	
	// Typecheck current tree node - verify if children's types agree to expected types
	listCompare(ast->expectedTypes, 

}

int lookupSymbolsTable(char* token)
{
	// Look for the token in the symbol's table in the current 
}

bool typeInference(comp_tree_t* exp)
{
	
}

bool checkDeclaration(char* identifier)
{
	// check if variable is declared in the current context


	// if not, checks if the variable is declared in the parent context, recursively

}
