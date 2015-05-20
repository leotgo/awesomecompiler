#include "cc_label.h"

struct label_list* lbl_list = NULL;
int label_count = 0;
int label_name_size = 3;
int label_range = 10;

void label_list_add(char* name) {
	label_list* new = (label_list*)malloc(sizeof(label_list));
	new->name = name;
	new->next = lbl_list;
}

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
	
	label_list_add(name);
	free(buffer);
	return name;
}

void label_list_destroy()
{
	while (lbl_list != NULL) 
	{
		label_list* y = lbl_list;
		free(y->name);
		lbl_list = lbl_list->next;
		free(y);
	}
}
