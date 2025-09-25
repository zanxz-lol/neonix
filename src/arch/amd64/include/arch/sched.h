#ifndef __ARCH_SCHED_H
#define __ARCH_SCHED_H

#include <x86_64/intr.h>

struct arch_context {
    struct intr_stack_frame context;
};

void arch_setup_context(struct arch_context * ctx, void * entry, void * stack);
void arch_first_context_switch(struct arch_context * context);

#endif
