#ifndef __LIB_STRING_H
#define __LIB_STRING_H

#include <stdint.h>
#include <stddef.h>

void * memset(void const * dest, uint8_t data, size_t size);
int memcmp(const void * ptr1, const void * ptr2, size_t count);
void * memcpy(void * dest, const void * src, size_t size);
size_t strlen(const char * string);
char * strcpy(char * dest, const char * src);
char * strncpy(char * dest, const char * src, size_t size);

#endif
