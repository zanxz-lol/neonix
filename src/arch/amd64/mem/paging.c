#include <naho/math.h>
#include <naho/compiler.h>
#include <naho/proc.h>
#include <arch/operations.h>
#include <naho/printk.h>
#include <naho/kernel.h>
#include <naho/mem.h>
#include <x86_64/mem.h>
#include <x86_64/intr.h>
#include <lib/limine.h>
#include <lib/string.h>
#include <lib/bool.h>
#include <stddef.h>
#include <stdint.h>

static uint64_t * pml4_t __align(4096);

static uint64_t kernel_phys = 0;
static uint64_t kernel_virt = 0;
static uint64_t virt_offset = 0;

static uint64_t get_fault_addr(void) {
    uint64_t cr2;
    asm volatile("movq %%cr2, %0" : "=r"(cr2));
    return cr2;
}

static void page_fault_handler(struct intr_stack_frame * regs) {
    arch_disable_interrupts();
    uint64_t addr = get_fault_addr();
    panic("Page fault @ 0x%p\n", addr);
}

static void load_new_pml4(uint64_t pml4_addr) {
    printk("Loading new PML4 table @ 0x%p\n", pml4_addr);
    asm volatile("movq %0, %%cr3" :: "r"(pml4_addr));
}

void sys_paging_init(void) {
    intr_add_handler(X86_TRAP_PF, page_fault_handler);
    kernel_phys = boot_info.krnl_phys_addr;
    kernel_virt = boot_info.krnl_virt_addr;
    virt_offset = boot_info.virt_offset;
    pml4_t = arch_alloc_pages(1);
    printk("Setting up page tables..\n");
    /* map memory */
    for(size_t i = 0; i < boot_info.mmap_entries; i++) {
        struct limine_memmap_entry * entry = boot_info.mmap[i];
        if (entry->type == LIMINE_MEMMAP_BAD_MEMORY || entry->type == LIMINE_MEMMAP_ACPI_NVS || entry->type == LIMINE_MEMMAP_RESERVED) {
            continue;
        }
        uint64_t flags = (PAGE_PRESENT | PAGE_RW);
        if (entry->type == LIMINE_MEMMAP_FRAMEBUFFER) {
            flags |= (PAGE_WRITETHROUGH | PAGE_USERMODE);
        }
        virtmem_map(virt_offset + entry->base, entry->base, entry->length, flags);
    }
    /* map read-only sections */
    uint64_t phys_base = kernel_phys + (boot_info.kernel_start - kernel_virt);
    virtmem_map(boot_info.kernel_start, phys_base, boot_info.data_section_start - boot_info.kernel_start, PAGE_PRESENT);
    /* map read-write sections */
    phys_base = kernel_phys + (boot_info.data_section_start - kernel_virt);
    virtmem_map(boot_info.data_section_start, phys_base, boot_info.kernel_end - boot_info.data_section_start, PAGE_RW | PAGE_PRESENT);
    virtmem_map(virt_offset, 0, 1, 0);
    /* load new pml4 */
    load_new_pml4((uint64_t)pml4_t - virt_offset);
    printk("New PML4 table loaded.\n");
}

void * arch_alloc_pages(size_t blocks) {
    void * page_virt = (void *)(virt_offset + physmem_alloc_blocks(blocks));
    memset(page_virt, 0, PAGE_SIZE);
    return page_virt;
}

void arch_free_pages(void * ptr, size_t blocks) {
    uint64_t phys_ptr = (((uint64_t)ptr) - virt_offset);
    physmem_free_blocks((void *)phys_ptr, blocks);
}

/*
* I'm not going crazy.. No...
*/

static void _virtmem_map(uint64_t * pagedir, uint64_t virt_addr, const uint64_t phys_addr, const uint64_t flags) {
    if (virt_addr % PAGE_SIZE) {
        printk("0x%p is unaligned.\n", virt_addr);
        return;
    }
    virt_addr = ADDR_48BIT(virt_addr);
    size_t pml4_index = PML4_INDEX(virt_addr);
    size_t pdp_index = PDP_INDEX(virt_addr);
    size_t pd_index = PD_INDEX(virt_addr);
    size_t pt_index = PT_INDEX(virt_addr);
    /* level 4 */
    if (TEST_PAGE_FLAG(pagedir[pml4_index], PAGE_PRESENT) == false) {
        void * addr = physmem_alloc_block();
        pagedir[pml4_index] = ((uint64_t)(addr) | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE);
    }
    /* level 3 */
    uint64_t * pdp_t = (uint64_t *)(virt_offset + PAGE_GET_PHYS_ADDR(pagedir[pml4_index]));
    if (TEST_PAGE_FLAG(pdp_t[pdp_index], PAGE_PRESENT) == false) {
        void * addr = physmem_alloc_block();
        pdp_t[pdp_index] = ((uint64_t)(addr) | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE);
    }
    /* level 2 */
    uint64_t * pd_t = (uint64_t *)(virt_offset + PAGE_GET_PHYS_ADDR(pdp_t[pdp_index]));
    if (TEST_PAGE_FLAG(pd_t[pd_index], PAGE_PRESENT) == false) {
        void * addr = physmem_alloc_block();
        pd_t[pd_index] = ((uint64_t)(addr) | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE);
    }
    /* level 1 */
    uint64_t * pt = (uint64_t *)(virt_offset + PAGE_GET_PHYS_ADDR(pd_t[pd_index]));
    pt[pt_index] = ((uint64_t)PAGE_PHYS_ADDR(phys_addr)) | flags;
    invalidate(virt_addr);
}

void __hot virtmem_map(const uint64_t virt_addr, const uint64_t phys_addr, const size_t length, const uint64_t flags) {
    size_t pages = DIV_CEILING(length, PAGE_SIZE);
    for(size_t i = 0; i < pages; i++) {
        _virtmem_map(pml4_t, virt_addr + (i * PAGE_SIZE), phys_addr + (i * PAGE_SIZE), flags);
    }
}
