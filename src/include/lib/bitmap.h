#ifndef __LIB_BITMAP_H
#define __LIB_BITMAP_H

#include <neonix/math.h>
#include <lib/bool.h>
#include <stddef.h>
#include <stdint.h>

#define BITS_TO_BYTES(x) ((x) / 8)
#define BIT_OFFSET(x) ((x) % 8)

struct bitmap {
    uint8_t * data;
    size_t size;
};

/* basic operations */
bool bitmap_test(struct bitmap * bitmap, size_t bit);
void bitmap_set(struct bitmap * bitmap, size_t bit);
void bitmap_unset(struct bitmap * bitmap, size_t bit);
int bitmap_find_first_free_bit(struct bitmap * bitmap);
int bitmap_find_free_bits(struct bitmap * bitmap, size_t size);

#endif
