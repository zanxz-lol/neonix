#ifndef __NAHO_KERNEL_H
#define __NAHO_KERNEL_H

#include <naho/init.h>
#include <lib/limine.h>
#include <stdint.h>
#include <stddef.h>

extern initcall_entry_t initcall0_start[];
extern initcall_entry_t initcall1_start[];
extern initcall_entry_t initcall2_start[];
extern initcall_entry_t initcalls_end[];

extern uint64_t text_start;
extern uint64_t text_end;
extern uint64_t rodata_start;
extern uint64_t rodata_end;
extern uint64_t data_start;
extern uint64_t data_end;
extern uint64_t bss_start;
extern uint64_t bss_end;
extern uint64_t kernel_start;
extern uint64_t kernel_end;

struct boot_information {
    uint64_t rsdp_ptr;
    uint64_t virt_offset;
    uint64_t krnl_virt_addr;
    uint64_t krnl_phys_addr;
    /* ELF sections and linker variables */
    uint64_t kernel_start;
    uint64_t text_section_start;
    uint64_t text_section_end;
    uint64_t rodata_section_start;
    uint64_t rodata_section_end;
    uint64_t data_section_start;
    uint64_t data_section_end;
    uint64_t bss_section_start;
    uint64_t bss_section_end;
    uint64_t kernel_end;

    size_t total_mem;
    size_t avail_mem;
    size_t mmap_entries;
    size_t fb_entries;
    struct limine_framebuffer ** fb_list;
    struct limine_memmap_entry ** mmap;
};

struct boot_information boot_info;

#endif
