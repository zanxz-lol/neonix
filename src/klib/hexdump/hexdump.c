#include <neonix/printk.h>
#include <lib/ctype.h>
#include <lib/common.h>
#include <stdint.h>
#include <stddef.h>

void hexdump(const void * ptr, size_t len) {
    uint64_t address = (uint64_t)ptr;
    size_t count;

    for (count = 0 ; count < len; count += 16) {
        printk("0x%p: ", address);
        printk("%08zx %08zx %08zx %08zx |", *(const uint32_t *)address, *(const uint32_t *)(address + 4), *(const uint32_t *)(address + 8), *(const uint32_t *)(address + 12));
        for (int i = 0; i < 16; i++) {
            char c = *(const char *)(address + i);
            if (isalpha(c)) {
                printk("%c", c);
            } else {
                printk(".");
            }
        }
        printk("|\n");
        address += 16;
    }	
}
