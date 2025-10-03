#ifndef __ARCH_OPS_H
#define __ARCH_OPS_H

#include <naho/compiler.h>
#include <stddef.h>
#include <stdint.h>

#include <config.h>

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

#ifdef CONFIG_NAHO_DEBUG

#ifdef CONFIG_DEBUGCON_ENABLE

void arch_dbg_putc(const char ch);
void arch_dbg_printf(const char * fmt, ...);

#else

#define arch_dbg_putc(ch)
#define arch_dbg_printf(fmt, ...)

#endif /* CONFIG_DEBUGCON_ENABLE */

#endif /* CONFIG_NAHO_DEBUG */
/* fun stuff */

#ifdef CONFIG_NAHO_FUNTIME
void arch_bad_apple(void);
#endif

#endif
