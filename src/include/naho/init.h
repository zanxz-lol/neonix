#ifndef __NAHO_INIT_H
#define __NAHO_INIT_H

#include <naho/stringify.h>
#include <naho/compiler.h>

/* heavily inspired by linux's implementation of initcalls, but with some changes */


typedef int (*initcall_t)(void);
typedef int initcall_entry_t;

enum {
    INITCALL_ARCH_SPECIFIC,
    INITCALL_DRIVER,
    INITCALL_FILESYSTEM,
    INITCALL_LENGTH
};

#define DEFINE_INITCALL(func, type) \
    static initcall_t __used __section(__stringify(.initcall##type.init)) __initcall_decl##__COUNTER__ = func;

#define DRIVER_INITCALL(func) \
    DEFINE_INITCALL(func, 1)

void initcalls_init(void);

#endif
