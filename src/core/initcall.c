#include <naho/compiler.h>
#include <naho/kernel.h>
#include <naho/printk.h>
#include <naho/init.h>
#include <lib/string.h>
#include <stddef.h>

static const char initcall_lut[][16] = {
    "arch-specific",
    "driver",
    "filesystem"
};

static initcall_entry_t * initcall_types[] = {
    initcall0_start,
    initcall1_start,
    initcall2_start,
    initcalls_end
};

static inline void * entry_to_initcall(const int * offset) {
	return (void *)((uintptr_t)*offset);
}

static void initcall_call_single(initcall_t func) {
    /* bounds check */
    if ((uintptr_t)func > (uintptr_t)&kernel_end) {
        return;
    }
    printk("Calling initcall @ 0x%p\n", func);
    int rc = func();
    printk("Initcall returned %d\n", rc);
}

static void initcall_call_type(int type) {
    initcall_entry_t * func = initcall_types[type];
    if (func == initcall_types[type + 1]) {
        return;
    }
    printk("Calling %s initcalls..\n", initcall_lut[type]);
    while(func < initcall_types[type + 1]) {
        initcall_call_single((initcall_t)entry_to_initcall(func));
        func++;
    }
}

static void initcall_call_all_types(void) {
    for(size_t i = 0; i < INITCALL_LENGTH; i++) {
        initcall_call_type(i);
    }
}

void initcalls_init(void) {
    initcall_call_all_types();
}
