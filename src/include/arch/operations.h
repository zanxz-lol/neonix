#ifndef __ARCH_OPS_H
#define __ARCH_OPS_H

#define CONFIG_NEONIX_FUNTIME 1

#include <neonix/compiler.h>
#include <stddef.h>
#include <stdint.h>

/* normal stuff */

int arch_early_init(void);
void * arch_memset(void const * dest, uint8_t data, size_t size);
uint64_t arch_random(void);
void arch_enable_interrupts(void);
void arch_disable_interrupts(void);
void __noreturn arch_die(void);

/* memory */

void * arch_alloc_pages(size_t blocks);
void arch_free_pages(void * ptr, size_t blocks);

/* debugging */

void arch_dbg_putc(const char ch);

/* fun stuff */

void arch_bad_apple(void);

#endif
