#include <lib/string.h>

char * strncpy(char * dest, const char * src, size_t size) {
    memcpy(dest, src, size);
    dest[size] = 0;
    return dest;
}
