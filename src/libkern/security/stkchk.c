#include <arch/operations.h>
#include <naho/compiler.h>
#include <naho/printk.h>
#include <naho/stkchk.h>
#include <stdint.h>

uint64_t __stack_chk_guard;

/* we dont want to trigger the stack protector early on */
void __nostackcheck __stack_chk_guard_init(uint64_t rand) {
    __stack_chk_guard = rand;
}

void __noreturn __stack_chk_fail(void) {
    printk("Stack smashing detected @ 0x%p.\n", __builtin_return_address (0));
    arch_die();
}
