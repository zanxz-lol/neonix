#include <neonix/kernel.h>
#include <neonix/printk.h>
#include <x86_64/acpi.h>
#include <x86_64/apic.h>
#include <x86_64/mem.h>
#include <lib/string.h>
#include <lib/bool.h>
#include <stddef.h>
#include <stdint.h>

static struct xsdp * xsdp;
static struct xsdt * xsdt;

static bool is_checksum_valid(struct acpi_sdt_header * header) {
    uint8_t sum = 0;
    for (size_t i = 0; i < header->length; i++) {
        sum += ((char *)header)[i];
    }
    return sum == 0;
}

static void * acpi_search_for_table(char * signature) {
    size_t entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;
    for(size_t i = 0; i < entries; i++) {
        struct acpi_sdt_header * header = (struct acpi_sdt_header *)(boot_info.virt_offset + xsdt->pointers[i]);
        if (memcmp(header->signature, signature, 4) == 0) {
            return (void *)header;
        }
    }
    return NULL;
}

int sys_acpi_init(void) {
    /* we need acpi to detect the APIC */
    xsdp = (struct xsdp *)(boot_info.rsdp_ptr);
    if (xsdp->revision < 2) {
        printk("NEONIX does not support devices with a ACPI version less than 2\n");
        return -1;
    }
    xsdt = (struct xsdt *)(boot_info.virt_offset + xsdp->xsdt_addr);
    bool checksum_valid = is_checksum_valid(&xsdt->header);
    if (checksum_valid == false) {
        printk("XSDT checksum is INVALID!\n");
        return -1;
    }
    size_t entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;
    printk("Total ACPI tables: %d\n", entries);
    void * fadt = acpi_search_for_table("FACP");
    if (fadt == NULL) {
        printk("FADT is NULL!\n");
        return -1;
    }
    sys_acpi_enable(fadt);
    void * madt = acpi_search_for_table("APIC");
    if (madt == NULL) {
        return -1;
    }
    sys_apic_init(madt);
    return 0;
}
