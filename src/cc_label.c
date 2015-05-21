#include "cc_label.h"

int label_count = 0;
int label_name_size = 3;
int label_range = 10;

char* generate_label()
{

	char *name = (char*) malloc(label_name_size);
	char * buffer = (char*) malloc(label_name_size);
	strcpy(name, "L");
	sprintf(buffer,"%d",label_count);
	strcat(name, buffer);
	label_count++;
	int i;
	if(label_count >= label_range)
	{
		label_name_size++;
		label_range*=10;
	}
	
	free(buffer);
	str_pool_add(name);
	return name;
}

