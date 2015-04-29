#ifndef __TYPE_H
#define __TYPE_H

#define IKS_INVALID 0
#define IKS_INT 1
#define IKS_FLOAT 2
#define IKS_CHAR 3
#define IKS_STRING 4
#define IKS_BOOL 5

// Type list structure for AST nodes, used to determine whether the
// node's children agree to expected types.
typedef struct type_list {
	int type;
	t_list* next;
} type_list;

// Adds a type to the specified type list
type_list* type_list_Add(type_list* list);

// Compares two type lists and returns true if they are 
// identical - same elements in same order
bool type_list_Compare(type_list* list_a, type_list* list_b);

#endif
