#ifndef __X86_64_PIC_H
#define __X86_64_PIC_H

#include <stdint.h>

void lapic_send_eoi(void);
void sys_apic_init(void * table);
void sys_disable_legacy_i8259(void);

#endif
