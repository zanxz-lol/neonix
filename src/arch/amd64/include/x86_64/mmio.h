#ifndef __X86_64_MMIO_H
#define __X86_64_MMIO_H

#include <stdint.h>

static inline uint8_t mmio_read8(void * addr) {
    volatile uint8_t data = *(volatile uint8_t *)addr;
    return data;
}

static inline uint16_t mmio_read16(void * addr) {
    volatile uint16_t data = *(volatile uint16_t *)addr;
    return data;
}

static inline uint32_t mmio_read32(void * addr) {
    volatile uint32_t data = *(volatile uint32_t *)addr;
    return data;
}

static inline void mmio_write8(void * addr, uint8_t data) {
    *(volatile uint16_t *)addr = data; 
}

static inline void mmio_write16(void * addr, uint16_t data) {
    *(volatile uint16_t *)addr = data; 
}

static inline void mmio_write32(void * addr, uint32_t data) {
    *(volatile uint32_t *)addr = data; 
}

#endif
