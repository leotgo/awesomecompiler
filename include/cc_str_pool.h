#ifndef STR_POOL_H
#define STR_POOL_H

void str_pool_add_lit(const char* str);

void str_pool_add(char* str);

void str_pool_destroy();

#endif