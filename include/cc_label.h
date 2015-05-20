#ifndef CC_LABEL_H
#define CC_LABEL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct label_list 
{
	char* name;
	struct label_list* next;
} label_list;

extern label_list* lbl_list;

void label_list_destroy();

char* generate_label();


#endif