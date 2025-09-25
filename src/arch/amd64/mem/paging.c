#include <neonix/proc.h>
#include <arch/operations.h>
#include <neonix/printk.h>
#include <neonix/kernel.h>
#include <neonix/mem.h>
#include <x86_64/mem.h>
#include <x86_64/intr.h>
#include <lib/bool.h>
#include <stddef.h>
#include <stdint.h>

static uint64_t * pml4;
static uint64_t virt_offset;

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

void sys_paging_init(void) {
    uint64_t raw_cr3 = 0;
    asm volatile("movq %%cr3, %0" : "=r"(raw_cr3));
    uint64_t pml4_phys = raw_cr3;
    uint64_t pml4_virt = (boot_info.virt_offset + pml4_phys);
    printk("PML4 physical address: 0x%p\n", pml4_phys);
    printk("PML4 virtual address: 0x%p\n", pml4_virt);
    pml4 = (uint64_t *)pml4_virt;
    virt_offset = boot_info.virt_offset;
    intr_add_handler(X86_TRAP_PF, page_fault_handler);
}

void * arch_alloc_pages(size_t blocks) {
    return (virt_offset + physmem_alloc_blocks(blocks)) ;
}

void arch_free_pages(void * ptr, size_t blocks) {
    uint64_t phys_ptr = (((uint64_t)ptr) - virt_offset);
    physmem_free_blocks((void *)phys_ptr, blocks);
}

/*
*   To test if im really going crazy, i gotta test out other peoples code
*/

static void _virtmem_map(uint64_t * pagedir, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags) {
    if (virt_addr % PAGE_SIZE) {
        printk("Unaligned address\n");
        arch_die();
    }
    virt_addr = ADDR_48BIT(virt_addr);
    uint32_t pml4_index = PML4_INDEX(virt_addr);
    uint32_t pdp_index = PDP_INDEX(virt_addr);
    uint32_t pd_index = PD_INDEX(virt_addr);
    uint32_t pt_index = PT_INDEX(virt_addr);
    if (!(pagedir[pml4_index] & PAGE_PRESENT)) {
        size_t target = (size_t)physmem_alloc_block();
        pagedir[pml4_index] = target | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE;
    }
    size_t *pdp = (size_t *)(PAGE_GET_PHYS_ADDR(pagedir[pml4_index]) + boot_info.virt_offset);

    if (!(pdp[pdp_index] & PAGE_PRESENT)) {
        size_t target = (size_t)physmem_alloc_block();
        pdp[pdp_index] = target | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE;
    }
    size_t *pd = (size_t *)(PAGE_GET_PHYS_ADDR(pdp[pdp_index]) + boot_info.virt_offset);

    if (!(pd[pd_index] & PAGE_PRESENT)) {
        size_t target = (size_t)physmem_alloc_block();
        pd[pd_index] = target | PAGE_PRESENT | PAGE_RW | PAGE_USERMODE;
    }
    size_t *pt = (size_t *)(PAGE_GET_PHYS_ADDR(pd[pd_index]) + boot_info.virt_offset);
    if (!phys_addr) // todo: proper unmapping
        pt[pt_index] = 0;
    else
        pt[pt_index] = (PAGE_PHYS_ADDR(phys_addr)) | PAGE_PRESENT | flags; // | PAGE_RW

    invalidate(virt_addr);
}

void virtmem_map(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags) {
    _virtmem_map(pml4, virt_addr, phys_addr, flags);
}
