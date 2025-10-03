#include <arch/operations.h>
#include <lib/string.h>

int memcmp(const void * ptr1, const void * ptr2, size_t count) {
    const uint8_t * s1 = (const uint8_t *)ptr1;
    const uint8_t * s2 = (const uint8_t *)ptr2;

    while (count-- > 0)
    {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}
