
#include "cc_register.h"

char* generate_register()
{

	char *name = (char*) malloc(register_name_size);
	char * buffer = (char*) malloc(register_name_size);
	strcpy(name, "r");
	sprintf(buffer,"%d",register_count);
	strcat(name, buffer);
	register_count++;
	int i;
	if(register_count >= range)
	{
		register_name_size++;
		range*=10;
	}
	
	register_list_add(name);
	free(buffer);
	return name;
}

void register_list_add(char* name)
{
	register_list* new = (register_list*)malloc(sizeof(register_list));
	new->name = name;
	new->next = reg_list;
}

void register_list_destroy()
{
	while (reg_list != NULL) 
	{
		register_list* y = reg_list;
		reg_list = reg_list->next;
		free(y);
	}
}
