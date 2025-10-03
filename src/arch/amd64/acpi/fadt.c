#include <naho/printk.h>
#include <x86_64/acpi.h>
#include <x86_64/pmio.h>
#include <lib/bool.h>

static struct fadt * fadt;

static inline bool is_acpi_enabled(void) {
    return ((pmio_read16(fadt->PM1aControlBlock) & 1) == 1);
}

void sys_acpi_enable(void * table) {
    printk("Enabling ACPI...\n");
    fadt = table;
    if (is_acpi_enabled() == true) {
        printk("ACPI is already enabled by the BIOS :)\n");
        return;
    }
    printk("Enabling ACPI mode..\n"); 
    pmio_write8(fadt->sys_management_port, fadt->acpi_enable);
    while(is_acpi_enabled() == false);
    printk("ACPI is now enabled :)\n");
}
