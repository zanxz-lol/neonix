#ifndef __X86_64_COMMON_H
#define __X86_64_COMMON_H

#include <stdint.h>

#define KRNL_CODE_SEGM 0x28
#define KRNL_DATA_SEGM 0x30

static inline void wrmsr(uint32_t msr, uint64_t data) {
    uint32_t low = data & 0xffffffff;
    uint32_t high = (data >> 32) & 0xffffffff;
    asm volatile("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

#endif
