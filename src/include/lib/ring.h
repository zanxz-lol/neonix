#ifndef __LIB_RING_BUFFER_H
#define __LIB_RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>

struct ring_buffer {
    size_t head;
    size_t tail;
    size_t size;
    size_t count;
    uint8_t * buffer;
};

void ring_buffer_write(struct ring_buffer * ring, uint8_t data);

#endif
