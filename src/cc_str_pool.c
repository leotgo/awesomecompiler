#include "cc_str_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
typedef struct str_pool {
	char* str;
	struct str_pool* next;
} str_pool;

str_pool* pool = NULL;

char* str_pool_lit(const char* str, ...) {
	char* s = (char*)malloc(sizeof(char) * (10000));
	va_list va; va_start(va, str);
	vsprintf(s, str, va);
	va_end(va);
	str_pool_add(s);
	return s;
}

char* str_pool_add(char* str) {
	str_pool* p = pool;
	pool = (str_pool*)malloc(sizeof(str_pool));
	pool->next = p;
	pool->str = str;
	return str;
}

void str_pool_destroy() {
	str_pool* p = pool;
	while (p) {
		str_pool* d = p;
		p = p->next;
		free(d->str);
		free(d);		
	}
}



