#include <lib/ring.h>

void ring_buffer_write(struct ring_buffer * ring, uint8_t data) {
    if (ring->count == ring->size) {
        return;
    }
    ring->tail %= ring->size;
    ring->buffer[ring->tail++] = data;
    ring->count++;
}
