#include <lib/string.h>

size_t strlen(const char * string) {
    size_t size;
    for(size = 0; string[size] != 0; size++);
    return size;
}
