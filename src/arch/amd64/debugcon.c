#include <arch/operations.h>
#include <x86_64/pmio.h>
#include <lib/bool.h>

#define DEBUGCON_IO 0xE9

static inline bool sys_has_debugcon(void) {
    return (pmio_read8(DEBUGCON_IO) == (uint8_t)DEBUGCON_IO);
}

static inline void debugcon_putc(const char ch) {
    if (sys_has_debugcon() == false) {
        return;
    }
    pmio_write8(DEBUGCON_IO, ch);
}

void arch_dbg_putc(const char ch) {
    debugcon_putc(ch);
}
