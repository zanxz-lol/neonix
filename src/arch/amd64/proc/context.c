#include <arch/sched.h>
#include <arch/operations.h>
#include <neonix/proc.h>
#include <neonix/compiler.h>
#include <x86_64/common.h>
#include <x86_64/sched.h>
#include <x86_64/apic.h>
#include <x86_64/intr.h>
#include <lib/string.h>

extern void __noreturn perform_first_context_switch(uint64_t rsp);

void schedule(struct intr_stack_frame * regs) {
    arch_disable_interrupts();
    struct proc * curr = get_current_proc(); 
    if (curr->cpu_time_left >= 1) {
        curr->cpu_time_left--;
        arch_enable_interrupts();
        return;
    }
    curr->cpu_time_left = curr->time_quantum;
    lapic_send_eoi();
    memcpy(&curr->context.context, regs, sizeof(struct intr_stack_frame));
    struct proc * next = find_next_proc();
    curr = next;
    if (curr->state == PROC_CREATED) {
        curr->state = PROC_ALIVE;
        arch_first_context_switch(&curr->context);
    }
    memcpy(regs, &curr->context.context, sizeof(struct intr_stack_frame));
    arch_enable_interrupts();
}

void arch_setup_context(struct arch_context * ctx, void * entry, void * stack) {
    ctx->context.cs = KRNL_CODE_SEGM;
    ctx->context.ds = KRNL_DATA_SEGM;
    ctx->context.ss = KRNL_DATA_SEGM;
    ctx->context.eflags = 0x202;
    ctx->context.rax = 0;
    ctx->context.rcx = 0;
    ctx->context.rdx = 0;
    ctx->context.rbx = 0;
    ctx->context.rsi = 0;
    ctx->context.rdi = 0;
    ctx->context.rsp = (uint64_t)(stack + 4096);
    ctx->context.rsp -= sizeof(struct intr_stack_frame);
    ctx->context.rbp = 0;
    ctx->context.rip = (uint64_t)entry;
    ctx->context.r8 = 0;
    ctx->context.r9 = 0;
    ctx->context.r10 = 0;
    ctx->context.r11 = 0;
    ctx->context.r12 = 0;
    ctx->context.r13 = 0;
    ctx->context.r14 = 0;
    ctx->context.r15 = 0;
    memcpy((void *)ctx->context.rsp, &ctx->context, sizeof(struct intr_stack_frame));
}

void __noreturn arch_first_context_switch(struct arch_context * context) {
    perform_first_context_switch(context->context.rsp);
    /* die if this is ever reached */
    arch_die();
}
