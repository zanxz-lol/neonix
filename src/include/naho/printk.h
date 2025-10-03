#ifndef __NAHO_PRINTK_H
#define __NAHO_PRINTK_H

#include <arch/operations.h>
#include <lib/printf.h>

#define panic(...) \
    printk("Kernel panic - " __VA_ARGS__); \
    arch_die()

void printk_copy_buffer_to_console(void);
void printk(const char * fmt, ...);

#endif
