#include "cc_register.h"
#include "cc_str_pool.h"

int register_count = 0;
int register_name_size = 3;
int range = 10;

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
	
	free(buffer);
	str_pool_add(name);
	return name;
}

char* reg_fp() {
	static char* rfp = "rfp";
	return rfp;
}

char* reg_arp() {
	static char* rarp = "rarp";
	return rarp;
}
