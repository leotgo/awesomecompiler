
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct label_list 
{
	char* name;
	struct label_list* next;
} label_list;

struct label_list* lbl_list;
int label_count = 0;
int label_name_size = 3;
int label_range = 10;

void label_list_add(char* name);

void label_list_destroy();

char* generate_label();
