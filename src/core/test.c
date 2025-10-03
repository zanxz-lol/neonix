#include <naho/init.h>
#include <naho/printk.h>

static int test(void) {
    printk("I am a test initcall. Nice to meet you! :)\n");
    return 0;
}

DRIVER_INITCALL(test);
