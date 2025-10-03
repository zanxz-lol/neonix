#include "arch/operations.h"
#include <lib/string.h>
#include <stdint.h>
#include <stddef.h>

void * memset(void const * dest, uint8_t data, size_t size) {
    return arch_memset(dest, data, size);
}
