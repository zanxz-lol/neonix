#include "neonix/proc.h"
#include <arch/operations.h>
#include <neonix/compiler.h>
#include <neonix/stkchk.h>
#include <neonix/kernel.h>
#include <neonix/printk.h>
#include <neonix/console.h>
#include <neonix/requests.h>
#include <neonix/init.h>
#include <neonix/mem.h>
#include <lib/limine.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/bool.h>

static void bootloader_collect_info(void) {
    boot_info.krnl_phys_addr = limine_krnl_addr_request.response->physical_base;
    boot_info.krnl_virt_addr = limine_krnl_addr_request.response->virtual_base;
    boot_info.virt_offset = limine_hhdm_request.response->offset;
    boot_info.rsdp_ptr = (uint64_t)limine_rsdp_request.response->address;
    boot_info.mmap_entries = limine_mmap_request.response->entry_count;
    boot_info.mmap = limine_mmap_request.response->entries;
    boot_info.fb_entries = limine_fb_request.response->framebuffer_count;
    boot_info.fb_list = limine_fb_request.response->framebuffers;
    for(size_t i = 0; i < boot_info.mmap_entries; i++) {
        if (boot_info.mmap[i]->type == LIMINE_MEMMAP_USABLE) {
            boot_info.avail_mem += boot_info.mmap[i]->length;
        }
        boot_info.total_mem += boot_info.mmap[i]->length;
    }
}

static void __noreturn neonix_idle(void);

void __noreturn __externally_visible neonix_entry(void) {
    /* setup kernel stack protector */
    __stack_chk_guard_init(arch_random());
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        panic("Limine base revision is unsupported.\n");
    }
    bootloader_collect_info();
    printk("\nNEONIX (New UNIX)\n");
    printk("\tA modern, and open-source alternative to typical UNIX-like systems\n\n");
    init_physmem_allocator();
    limine_fbcon_init();
    int rc = arch_early_init();
    if (rc < 0) {
        printk("Failed to initialize architecture-specific features.\n");
        arch_die();
    }
#ifdef CONFIG_NEONIX_FUNTIME
    printk("Bad Apple time >:)\n");
    arch_bad_apple();
#endif
    //create_idle_proc();
    for(;;);
}

