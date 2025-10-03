#include <arch/operations.h>
#include <x86_64/pmio.h>
#include <lib/printf.h>
#include <lib/bool.h>
#include <stdarg.h>

#include <config.h>

#ifdef CONFIG_NAHO_DEBUG

#ifdef CONFIG_DEBUGCON_ENABLE

#define DEBUGCON_IOPORT 0xE9

static inline bool sys_has_debugcon(void) {
    return (pmio_read8(DEBUGCON_IOPORT) == (uint8_t)DEBUGCON_IOPORT);
}

static inline void debugcon_putc(const char ch) {
    if (sys_has_debugcon() == false) {
        return;
    }
    pmio_write8(DEBUGCON_IOPORT, ch);
}

void arch_dbg_putc(const char ch) {
    debugcon_putc(ch);
}

void arch_dbg_printf(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    /* TODO: implement printk */
    char buf[1024];
    vsnprintf_(buf, 1023, fmt, ap);
    for(size_t i = 0; buf[i] != 0; i++) {
        arch_dbg_putc(buf[i]);
    }
    va_end(ap);
}

#endif /* CONFIG_DEBUGCON_ENABLE */

#endif /* CONFIG_NAHO_DEBUG */
