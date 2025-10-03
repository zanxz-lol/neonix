#include <lib/string.h>
#include <stddef.h>

void * memcpy(void * dest, const void * src, size_t size) {
    const char * sp = (const char *)src;
    char * dp = (char *)dest;
    for(; size != 0; size--) *dp++ = *sp++;
    return dest;
}
