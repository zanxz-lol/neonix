#include <arch/operations.h>
#include <naho/compiler.h>
#include <naho/stkchk.h>
#include <naho/kernel.h>
#include <naho/printk.h>
#include <naho/console.h>
#include <naho/requests.h>
#include <naho/errno.h>
#include <naho/init.h>
#include <naho/mem.h>
#include <lib/limine.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/bool.h>
#include <config.h>

static const char * mmap_types[] = {
    [LIMINE_MEMMAP_RESERVED]                = "Reserved",
    [LIMINE_MEMMAP_USABLE]                  = "Usable",
    [LIMINE_MEMMAP_ACPI_NVS]                = "ACPI NVS",
    [LIMINE_MEMMAP_BAD_MEMORY]              = "Bad memory",
    [LIMINE_MEMMAP_ACPI_RECLAIMABLE]        = "ACPI Reclaimable",
    [LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE]  = "Bootloader reclaimable",
    [LIMINE_MEMMAP_FRAMEBUFFER]             = "Framebuffer",
    [LIMINE_MEMMAP_KERNEL_AND_MODULES]      = "Kernel & Modules",
};

static void bootloader_collect_info(void) {
    memset(&boot_info, 0, sizeof(struct boot_information));
    boot_info.kernel_start = (uint64_t)&kernel_start;
    boot_info.text_section_start = (uint64_t)&text_start;
    boot_info.text_section_end = (uint64_t)&text_end;
    boot_info.rodata_section_start = (uint64_t)&rodata_start;
    boot_info.rodata_section_end = (uint64_t)&rodata_end;
    boot_info.data_section_start = (uint64_t)&data_start;
    boot_info.data_section_end = (uint64_t)&data_end;
    boot_info.bss_section_start = (uint64_t)&bss_start;
    boot_info.bss_section_end = (uint64_t)&bss_end;
    boot_info.kernel_end = (uint64_t)&kernel_end;
    boot_info.krnl_phys_addr = limine_krnl_addr_request.response->physical_base;
    boot_info.krnl_virt_addr = limine_krnl_addr_request.response->virtual_base;
    boot_info.virt_offset = limine_hhdm_request.response->offset;
    boot_info.rsdp_ptr = (uint64_t)limine_rsdp_request.response->address;
    boot_info.mmap_entries = limine_mmap_request.response->entry_count;
    boot_info.mmap = limine_mmap_request.response->entries;
    boot_info.fb_entries = limine_fb_request.response->framebuffer_count;
    boot_info.fb_list = limine_fb_request.response->framebuffers;
    boot_info.total_mem = 0;
    boot_info.avail_mem = 0;
    for(size_t i = 0; i < boot_info.mmap_entries; i++) {
        if (boot_info.mmap[i]->type == LIMINE_MEMMAP_USABLE) {
            boot_info.avail_mem += boot_info.mmap[i]->length;
        }
        boot_info.total_mem += boot_info.mmap[i]->length;
    }
}

void __noreturn __externally_visible naho_entry(void) {
    /* setup kernel stack protector */
    __stack_chk_guard_init(arch_random());
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        panic("Limine base revision is unsupported.\n");
    }
    printk("\nNaho Kernel %d.%d.%d\n", KERNEL_MAJOR_VERSION, KERNEL_MINOR_VERSION, KERNEL_PATCH_VERSION);
    printk("\tA modern, and open-source alternative to typical UNIX-like systems\n\n");
    bootloader_collect_info();
    init_physmem_allocator();
#ifdef CONFIG_NAHO_VIDEO
    limine_fbcon_init();
#endif
    int rc = arch_early_init();
    if (rc < 0) {
        printk("Failed to initialize architecture-specific features.\n");
        arch_die();
    }
#ifdef CONFIG_NAHO_FUNTIME
    printk("Bad Apple time >:)\n");
    arch_bad_apple();
    printk("I tried to warn you that enabling this option in the config was useless..\n");
#endif

#ifdef CONFIG_NAHO_DEBUG
    printk("INFO: This kernel was built with CONFIG_NAHO_DEBUG set.\n");
    printk("\tThis enables experimental features, and debugging features that may slow down the kernel.\n");
    printk("\tIf this is intentional, please, carry on. Otherwise, recompile the kernel without debug features.\n");
#endif
    initcalls_init();
    printk("Going idle..\n");
    for(;;);
}

