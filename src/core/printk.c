#include <arch/operations.h>
#include <neonix/console.h>
#include <neonix/printk.h>
#include <neonix/kernel.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/ring.h>
#include <stdarg.h>

static char printk_buffer[1024];
static struct ring_buffer printk_ring_buffer = {
    .head = 0,
    .tail = 0,
    .size = 1024,
    .count = 0,
    .buffer = (uint8_t *)&printk_buffer
};

void printk_copy_buffer_to_console(void) {
    for(size_t i = 0; i < printk_ring_buffer.count; i++) {
        console_putc(printk_buffer[i]);
    }
}

void printk(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    /* TODO: implement printk */
    char buf[1024];
    vsnprintf_(buf, 1024, fmt, ap);
    for(size_t i = 0; buf[i] != 0; i++) {
        if (is_console_available() == false) {
            arch_dbg_putc(buf[i]);
        } else {
            console_putc(buf[i]);
        }
        ring_buffer_write(&printk_ring_buffer, buf[i]);
    }
    va_end(ap);
}
