#include <neonix/printk.h>
#include <x86_64/gdt.h>
#include <stdint.h>

static struct gdtr gdtr;
static struct segm_desc gdt[8];

/* 16-bit */
#define SEGM16_CODE (GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW)
#define SEGM16_DATA (GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_RW)
/* 32-bit */
#define SEGM32_CODE SEGM16_CODE
#define SEGM32_DATA SEGM16_DATA
#define SEGM32_FLAGS (GDT_FLAG_GRANULARITY | GDT_FLAG_LEGACY)
/* 64-bit */
#define SEGM64_CODE SEGM32_CODE
#define SEGM64_DATA SEGM32_DATA
#define SEGM64_KRNL_CODE_FLAGS (GDT_FLAG_64BIT)

static void set_segm_descriptor(uint8_t segm, uint32_t limit, uint64_t base, uint8_t access, uint8_t flags) {
    gdt[segm].base16 = ((base) & 0xffff);
    gdt[segm].base24 = (((base) >> 16) & 0xff);
    gdt[segm].base32 = (((base) >> 24) & 0xff);
    gdt[segm].limit = ((limit) & 0xffff);
    gdt[segm].flags = ((flags) << 4) | (((limit) >> 16) & 0xf);
    gdt[segm].access = (access);
}

static inline void sys_load_gdtr(void) {
    asm volatile("lgdt %0\n\t"
        "push $0x28\n\t"
        "lea 1f(%%rip), %%rax\n\t"
        "push %%rax\n\t"
        "lretq\n\t"
        "1:\n\t"
        "mov $0x30, %%eax\n\t"
        "mov %%eax, %%ds\n\t"
        "mov %%eax, %%es\n\t"
        "mov %%eax, %%fs\n\t"
        "mov %%eax, %%gs\n\t"
        "mov %%eax, %%ss\n\t"
        :
        : "m"(gdtr)
        : "rax", "memory");
}

void sys_gdt_init(void) {
    /* null descriptor */
    set_segm_descriptor(0, 0, 0, 0, 0);
    /* 16-bit kernel code segment */
    set_segm_descriptor(1, 0xffff, 0, SEGM16_CODE, 0);
    /* 16-bit kernel data segment */
    set_segm_descriptor(2, 0xffff, 0, SEGM16_DATA, 0);
    /* 32-bit kernel code segment */
    set_segm_descriptor(3, 0xfffff, 0, SEGM32_CODE, SEGM32_FLAGS);
    /* 32-bit kernel data segment */
    set_segm_descriptor(4, 0xfffff, 0, SEGM32_DATA, SEGM32_FLAGS);
    /* 64-bit kernel code segment */
    /* NOTE: Limit and base are both ignored in 64-bit mode */
    set_segm_descriptor(5, 0, 0, SEGM64_CODE, SEGM64_KRNL_CODE_FLAGS);
    /* 64-bit kernel data segment */
    set_segm_descriptor(6, 0, 0, SEGM64_DATA, 0);
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.addr = (uint64_t)&gdt;
    printk("Loading GDTR @ 0x%p\n", &gdtr);
    sys_load_gdtr();
}
