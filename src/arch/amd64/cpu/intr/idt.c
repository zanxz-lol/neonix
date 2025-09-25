#include <neonix/printk.h>
#include <x86_64/common.h>
#include <x86_64/intr.h>
#include <stdint.h>

static struct idtr idtr;
static struct intr_gate idt[256];

#define IDT_NORMAL_INTR_GATE (INTR_GATE_PRESENT | INTR_GATE_DPL_RING3 | INTR_GATE_TYPE_NORMAL)
#define IDT_FATAL_INTR_GATE (INTR_GATE_PRESENT | INTR_GATE_DPL_RING3 | INTR_GATE_TYPE_FATAL)

static void set_intr_gate(uint8_t intr, uint16_t selector, uint64_t base, uint8_t ist, uint8_t flags) {
    idt[intr].segm = selector;
    idt[intr].addr16 = (uint16_t)((base) & 0xffff);
    idt[intr].addr32 = (uint16_t)(((base) >> 16) & 0xffff);
    idt[intr].addr64 = (uint32_t)(((base) >> 32) & 0xffffffff);
    idt[intr].ist = ist & 3;
    idt[intr].flags = flags;
}

static inline void sys_load_idtr(void) {
    asm volatile("lidt %0\n\t" :: "m"(idtr) : "memory");
}

void sys_idt_init(void) {
    for(uint16_t i = 0; i < sizeof(intr_stubs) / sizeof(intr_stubs[0]); i++) {
        set_intr_gate(i, KRNL_CODE_SEGM, intr_stubs[i], 0, (i < 32) ? IDT_FATAL_INTR_GATE : IDT_NORMAL_INTR_GATE);
    }
    idtr.addr = (uint64_t)&idt;
    idtr.limit = sizeof(idt) - 1;
    printk("Loading IDTR @ 0x%p\n", &idtr);
    sys_load_idtr();
}
