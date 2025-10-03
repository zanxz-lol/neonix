#include <arch/operations.h>
#include <naho/printk.h>
#include <naho/kernel.h>
#include <x86_64/timer.h>
#include <x86_64/apic.h>
#include <x86_64/gdt.h>
#include <x86_64/intr.h>
#include <x86_64/acpi.h>
#include <x86_64/mem.h>
#include <lib/bool.h>
#include <stdint.h>
#include <cpuid.h>

static void sys_enable_sse(void) {
    uint64_t cr0, cr4;
    /* adjust cr0 */
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= (1 << 1);
    cr0 &= ~(1 << 2);
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
    /* adjust cr4 */
    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= (1 << 9);
    cr4 |= (1 << 10);
    asm volatile("mov %0, %%cr4" :: "r"(cr4));
}

void arch_disable_interrupts(void) {
    asm volatile("cli");
}

void arch_enable_interrupts(void) {
    asm volatile("sti");
}

void __noreturn arch_die(void) {
    asm volatile("cli");
    asm volatile("hlt");
    for(;;);
}

static inline uint64_t save_rflags(void) {
    uint64_t rflags;
    asm volatile("pushfq\n\tpopq %0" : "=r"(rflags));
    return rflags;
}

static inline void load_rflags(uint64_t rflags) {
    asm volatile("pushq %0\n\tpopfq" :: "r"(rflags));
}

void * arch_memset(void const * dest, uint8_t data, size_t size) {
    uint64_t saved_rflags = save_rflags();
    asm volatile("cld\n\trep stosb\n\t" :: "a"(data), "c"(size), "d"(dest));
    load_rflags(saved_rflags);
    return (void *)dest;
}

static inline uint64_t xorshift32(uint64_t state) {
    // Simple xorshift PRNG
    state ^= (state << 13);
    state ^= (state >> 17);
    state ^= (state << 5);
    return state;
}

/* catk code but for 64-bit */
uint64_t arch_random(void) {
    uint32_t lo1, hi1, lo2, hi2;
    uint64_t entropy;

    // Read time-stamp counter once
    asm volatile ("rdtsc" : "=a"(lo1), "=d"(hi1));
    // Looks dumb, but read time-stamp counter again with a slight delay
    asm volatile ("rdtsc" : "=a"(lo2), "=d"(hi2));
    uint64_t ts_combined = ((uint64_t)hi1 << 32) | lo1;
    uint64_t ts_combined2 = ((uint64_t)hi2 << 32) | lo2;
    uint64_t combined_result = ts_combined ^ ts_combined2;

    asm volatile ("rdtsc" : "=a"(lo1), "=d"(hi1));
    entropy = (hi1 ^ lo1);

    // Combine the two sources of entropy
    uint64_t mixed_entropy = xorshift32(combined_result ^ entropy);

    return mixed_entropy;
}

int arch_early_init(void) {
    uint32_t eax, ecx, edx, ebx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    /* check for basic sse support */
    if ((edx & (1 << 25)) == false) {
        printk("Your system doesn't support SSE.\n");
        return -1;
    }
    /* check for latest features */
    if ((ecx & (1 << 19)) == false) {
        printk("Your system doesn't support the latest SSE features.\n");
        return -1;
    }
    /* TODO: Add support for AVX features */
    printk("Enabling SSE features..\n");
    sys_enable_sse();
    /* necessary things */
    sys_gdt_init();
    sys_idt_init();
    sys_paging_init();
    sys_disable_legacy_i8259();
    int rc = sys_acpi_init();
    if (rc < 0) {
        printk("Error while setting up ACPI\n");
        return -1;
    }
    sys_i8253_init();
    arch_enable_interrupts();
    return 0;
}
