#ifndef __X86_64_GDT_H
#define __X86_64_GDT_H

#include <naho/compiler.h>
#include <stdint.h>

struct gdtr {
    uint16_t limit;
    uint64_t addr;
} __packed;

struct segm_desc {
    uint16_t limit;
    uint16_t base16;
    uint8_t base24;
    uint8_t access;
    uint8_t flags;
    uint8_t base32;
} __packed;

/* access byte bits */

#define GDT_ACCESS_ACCESSED (1 << 0)
#define GDT_ACCESS_RW (1 << 1)
#define GDT_ACCESS_DC (1 << 2)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_SYSTEM (1 << 4)
#define GDT_ACCESS_USERMODE (3 << 5)
#define GDT_ACCESS_PRESENT (1 << 7)

/* flag bits */

#define GDT_FLAG_64BIT (1 << 1)
#define GDT_FLAG_LEGACY (1 << 2)
#define GDT_FLAG_GRANULARITY (1 << 3)

void sys_gdt_init(void);

#endif
