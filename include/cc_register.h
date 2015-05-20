#ifndef CC_REGISTER_H
#define CC_REGISTER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct register_list 
{
	char* name;
	struct register_list* next;
} register_list;

extern struct register_list* reg_list;

void register_list_add(char* name);

void register_list_destroy();

char* generate_register();


#endif