#include <stdio.h>

char* last_label = NULL;

char* generate_label()
{
	char* generated_label = NULL;

	if(last_label == NULL)
	{
		generated_label = (char*)malloc(sizeof(char) * (strlen("LABEL_0") + 2));
		strcpy(&generated_label, "LABEL_0");
		last_label = generated_label;
	}
	else
	{
		/* To be implemented: generate a label named 			*/
		/*                    LABEL_X where X is last_label + 1 	*/
	}

	return generated_label;
}
