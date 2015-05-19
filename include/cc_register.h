
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct register_list 
{
	char* name;
	struct register_list* next;
} register_list;

struct register_list* reg_list;
int register_count = 0;
int register_name_size = 3;
int range = 10;

void register_list_add(char* name);

void register_list_destroy();

char* generate_register();


