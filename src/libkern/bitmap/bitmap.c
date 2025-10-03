#include <naho/printk.h>
#include <lib/bitmap.h>
#include <lib/bool.h>
#include <stdint.h>

bool bitmap_test(struct bitmap * bitmap, size_t bit) {
    if (BITS_TO_BYTES(bit) > bitmap->size) {
        return true;
    }
    return bitmap->data[BITS_TO_BYTES(bit)] & (1 << (BIT_OFFSET(bit)));
}

void bitmap_set(struct bitmap * bitmap, size_t bit) {
    if (BITS_TO_BYTES(bit) > bitmap->size) {
        return;
    }
    bitmap->data[BITS_TO_BYTES(bit)] |= (1 << BIT_OFFSET(bit));
}

void bitmap_unset(struct bitmap * bitmap, size_t bit) {
    if (BITS_TO_BYTES(bit) > bitmap->size) {
        return;
    }
    bitmap->data[BITS_TO_BYTES(bit)] &= ~(1 << BIT_OFFSET(bit));
}

int bitmap_find_first_free_bit(struct bitmap * bitmap) {
    for(size_t i = 0; BITS_TO_BYTES(i) < bitmap->size; i++) {
        if (bitmap_test(bitmap, i) == false) {
            /* hooray, we found it */
            return i;
        }
    }
    /* no free bit found */
    return -1;
}

int bitmap_find_free_bits(struct bitmap * bitmap, size_t size) {
    size_t good_bits = 0;
    for(size_t i = 0; BITS_TO_BYTES(i) < bitmap->size; i++) {
        /* n++ reference? */
        for(size_t n = 0; n < size; n++) {
            if (bitmap_test(bitmap, i + n) == false) {
                if (good_bits == size) {
                    /* hooray, we found it */
                    return i;
                }
                good_bits++;
            } else {
                good_bits = 0;
            }
        }

    }
    /* no free bit found */
    return -1;
}
