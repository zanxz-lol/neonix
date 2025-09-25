#include <arch/operations.h>
#include <neonix/printk.h>
#include <neonix/kernel.h>
#include <x86_64/intr.h>
#include <x86_64/apic.h>

static intr_handler_func intr_handlers[256];

static void dump_info(struct intr_stack_frame * regs) {
    printk("--- Start of context dump ---\n");
    printk("Vector: 0x%02x, Error code: 0x%016zx\n", regs->intr, regs->error_code);
    printk("RAX: 0x%016zx RCX: 0x%016zx RDX: 0x%016zx RBX: 0x%016zx\n",
           regs->rax, regs->rcx, regs->rdx, regs->rbx);
    printk("RSI: 0x%016zx RDI: 0x%016zx RSP: 0x%016zx RBP: 0x%016zx\n",
           regs->rsi, regs->rdi, regs->rsp, regs->rbp);
    printk("R8: 0x%016zx R9: 0x%016zx R10: 0x%016zx R11: 0x%016zx\n",
           regs->r8, regs->r9, regs->r10, regs->r11);
    printk("R12: 0x%016zx R13: 0x%016zx R14: 0x%016zx R15: 0x%016zx\n",
           regs->r12, regs->r13, regs->r14, regs->r15);
    printk("RIP: 0x%016zx, SS: 0x%04x, DS: 0x%04x, CS: 0x%04x\n",
           regs->rip, regs->ss, regs->ds, regs->cs);
    printk("RFLAGS: 0x%016zx\n",
           regs->eflags);
    printk("--- End of context dump ---\n");
}

static int attempt_interrupt_handle(struct intr_stack_frame * regs) {
    intr_handler_func handler = intr_handlers[regs->intr];
    if (handler == NULL) {
        return -1;
    }
    handler(regs);
    return 0;
}

void intr_add_handler(uint8_t intr, intr_handler_func func) {
    if (intr_handlers[intr] != NULL) {
        printk("Conflicting interrupt handlers (0x%p vs 0x%p)\n", intr_handlers[intr], func);
        return;
    }
    intr_handlers[intr] = func;
}

void intr_handler(struct intr_stack_frame * regs) {
    int rc;
    rc = attempt_interrupt_handle(regs);
    if (regs->intr < 32) {
        if (rc < 0) {
            dump_info(regs);
            panic("Unhandled trap interrupt 0x%02x\n", regs->intr);
        }
    } else {
        if (rc < 0) {
            printk("Unimplemented interrupt 0x%02x\n", regs->intr);
        }
    }
}
