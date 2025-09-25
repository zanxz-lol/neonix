#ifndef __X86_PMIO_H
#define __X86_PMIO_H

#include <stdint.h>

static inline uint8_t pmio_read8(uint16_t port) {
    uint8_t data;
    asm volatile("inb %w1, %b0" : "=a" (data) : "Nd" (port) : "memory");
    return data;
}

static inline void pmio_write8(uint16_t port, uint8_t data) {
    asm volatile("outb %b0, %w1" :: "a" (data), "Nd" (port) : "memory");
}

static inline uint16_t pmio_read16(uint16_t port) {
    uint16_t data;
    asm volatile("inw %w1, %w0" : "=a" (data) : "Nd" (port) : "memory");
    return data;
}

static inline void pmio_write16(uint16_t port, uint16_t data) {
    asm volatile("outw %w0, %w1" :: "a" (data), "Nd" (port) : "memory");
}

static inline uint32_t pmio_read32(uint16_t port) {
    uint32_t data;
    asm volatile("inl %w1, %0" : "=a" (data) : "Nd" (port) : "memory");
    return data;
}

static inline void pmio_write32(uint16_t port, uint32_t data) {
    asm volatile("outl %0, %w1" :: "a" (data), "Nd" (port) : "memory");
}

#endif
