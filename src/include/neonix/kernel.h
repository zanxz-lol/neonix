#ifndef __NEONIX_KERNEL_H
#define __NEONIX_KERNEL_H

#include <neonix/init.h>
#include <lib/limine.h>
#include <stdint.h>
#include <stddef.h>

extern initcall_entry_t initcall0_start[];
extern initcall_entry_t initcall1_start[];
extern initcall_entry_t initcall2_start[];
extern initcall_entry_t initcalls_end[];

extern uint64_t kernel_start;
extern uint64_t kernel_end;

struct boot_information {
    uint64_t rsdp_ptr;
    uint64_t virt_offset;
    uint64_t krnl_virt_addr;
    uint64_t krnl_phys_addr;
    size_t total_mem;
    size_t avail_mem;
    size_t mmap_entries;
    size_t fb_entries;
    struct limine_framebuffer ** fb_list;
    struct limine_memmap_entry ** mmap;
};

struct boot_information boot_info;

#endif
