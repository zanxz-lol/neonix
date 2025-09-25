#ifndef __X86_64_MEM_H
#define __X86_64_MEM_H

#include <stdint.h>

#define PML4_INDEX(virt) ((virt) >> 39)
#define PDP_INDEX(virt) ((virt) >> 30)
#define PD_INDEX(virt) ((virt) >> 21)
#define PT_INDEX(virt) ((virt) >> 12)

/* page flags */
#define PAGE_PRESENT        (1 << 0)
#define PAGE_RW             (1 << 1)
#define PAGE_USERMODE       (1 << 2)
#define PAGE_WRITETHROUGH   (1 << 3)
#define PAGE_CACHE_DISABLE  (1 << 4)
#define PAGE_ACCESSED       (1 << 5)
#define PAGE_AVAILABLE      (1 << 6)

#define ADDR_48BIT(x) ((uint64_t)(x) & 0xFFFFFFFFFFFF)
#define PAGE_GET_PHYS_ADDR(x) ((x) & 0x000ffffffffff000)
#define PAGE_PHYS_ADDR(x) ((x) & ~0xfff)

#define PAGE_SIZE 4096

static inline void invalidate(uint64_t virt_addr) {
    asm volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}


void virtmem_map(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);
void sys_paging_init(void);

#endif
