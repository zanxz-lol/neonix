#ifndef __X86_64_SCHED_H
#define __X86_64_SCHED_H

#include <x86_64/intr.h>

void schedule(struct intr_stack_frame * regs);

#endif
