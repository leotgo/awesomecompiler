#include "cc_register.h"

struct register_list* reg_list = NULL;
int register_count = 0;
int register_name_size = 3;
int range = 10;

void register_list_add(char* name) {
	register_list* new = (register_list*)malloc(sizeof(register_list));
	new->name = name;
	new->next = reg_list;
}

char* generate_register()
{

	char *name = (char*) malloc(register_name_size);
	char * buffer = (char*) malloc(register_name_size);
	strcpy(name, "r");
	sprintf(buffer,"%d",register_count);
	strcat(name, buffer);
	register_count++;
	if(register_count >= range)
	{
		register_name_size++;
		range*=10;
	}
	
	register_list_add(name);
	free(buffer);
	return name;
}

void register_list_destroy()
{
	while (reg_list != NULL) 
	{
		register_list* y = reg_list;
		free(y->name);
		reg_list = reg_list->next;
		free(y);
	}
}

char* reg_fp() {
	static char* rfp = "rfp";
	return rfp;
}

char* reg_arp() {
	static char* rarp = "rarp";
	return rarp;
}
