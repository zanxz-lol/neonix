#include <naho/kernel.h>
#include <naho/printk.h>
#include <naho/mem.h>
#include <x86_64/common.h>
#include <x86_64/acpi.h>
#include <x86_64/mmio.h>
#include <x86_64/mem.h>
#include <lib/hexdump.h>
#include <lib/string.h>
#include <lib/list.h>
#include <stdint.h>

static uint64_t lapic_addr = 0;
static uint64_t ioapic_addr = 0;
static size_t irq_redirections = 0;

static struct madt * madt;
static struct madt_io_apic * ioapic;

static struct list lapic_list = { NULL, NULL };
static struct list ioapic_intr_overrides = { NULL, NULL };

#define IOAPIC_REGSEL 0x00
#define IOAPIC_REGWIN 0x10

#define IOAPIC_ID     0x00
#define IOAPIC_VER    0x01
#define IOAPIC_ARB    0x02
#define IOAPIC_TBL    0x10

void lapic_set_base(uint64_t lapic_addr) {
    wrmsr(0x1b, lapic_addr | 0x800 | 0x100);
}

static uint32_t ioapic_read(uint32_t reg) {
    void * addr = (void *)(ioapic_addr + boot_info.virt_offset);
    mmio_write32(addr + IOAPIC_REGSEL, reg);
    return mmio_read32(addr + IOAPIC_REGWIN);
}

static void ioapic_write(uint32_t reg, uint32_t data) {
    void * addr = (void *)(ioapic_addr + boot_info.virt_offset);
    mmio_write32(addr + IOAPIC_REGSEL, reg);
    mmio_write32(addr + IOAPIC_REGWIN, data);
}

static void ioapic_set_entry(uint8_t index, uint64_t data) {
    ioapic_write(0x10 + index * 2, (uint32_t)(data & 0xffffffff));
    ioapic_write(0x10 + index * 2 + 1, (uint32_t)((data >> 32) & 0xffffffff));
}

int ioapic_redirect_irq(uint8_t irq) {
    /* TODO: implement irq redirecting */
    struct list * curr = &ioapic_intr_overrides;
    while(curr != NULL) {
        struct madt_io_apic_iso * entry = (struct madt_io_apic_iso *)curr->data;
        if (entry->irq_src == irq) {
            printk("IO-APIC redirect IRQ %d to GSI %d\n", entry->irq_src, entry->gsi);
            irq = entry->gsi;
            break;
        }
        curr = curr->next;
    }
    return irq;
}

static void sys_ioapic_init(void) {
    size_t irq_capacity = (((ioapic_read(IOAPIC_VER) >> 16) & 0xff) + 1);
    printk("IO-APIC can handle %d IRQs\n", irq_capacity);
    for(size_t i = 0; i < irq_capacity; ++i) {
        ioapic_set_entry(i, 1 << 16);
    }
}

static uint32_t lapic_read(uint32_t reg) {
    void * addr = (void *)(lapic_addr + boot_info.virt_offset);
    return mmio_read32(addr + reg);
}

static void lapic_write(uint32_t reg, uint32_t data) {
    void * addr = (void *)(lapic_addr + boot_info.virt_offset);
    mmio_write32(addr + reg, data);
}

void lapic_send_eoi(void) {
    lapic_write(0xB0, 0x00);
}

static void sys_lapic_init(void) {
    lapic_set_base(lapic_addr);
    lapic_write(0x80, 0);
    lapic_write(0xe0, 0xffffffff);
    lapic_write(0xd0, 0x01000000);
    lapic_write(0xf0, lapic_read(0xf0) | 0x1ff);
}

void sys_apic_init(void * table) {
    madt = table;
    lapic_addr = madt->lapic_addr;
    printk("Local APIC MMIO address: 0x%p\n", lapic_addr);
    virtmem_map(boot_info.virt_offset + lapic_addr, lapic_addr, PAGE_SIZE, PAGE_PRESENT | PAGE_RW);
    uint8_t * ptr = (uint8_t *)(madt + 1);
    uint8_t * end = (uint8_t *)madt + madt->header.length;
    while (ptr < end) {
        struct acpi_entry_header * entry = (struct acpi_entry_header *)ptr;
        switch (entry->type) {
            case 0:
                list_append(&lapic_list, ptr);
                break;
            case 1:
                ioapic = (struct madt_io_apic *)ptr;
                printk("Found IO-APIC. IO-APIC ID = %d, MMIO base = 0x%08zx, GSI = 0x%08zx\n", 
                       ioapic->id, ioapic->ioapic_addr, ioapic->gsi_base);
                ioapic_addr = ioapic->ioapic_addr;
                virtmem_map(boot_info.virt_offset + ioapic_addr, ioapic_addr, PAGE_SIZE, PAGE_PRESENT | PAGE_RW);
                break;
            case 2:
                list_append(&ioapic_intr_overrides, ptr);
                break;
            default:
                break;
        }
        ptr += entry->length;
    }
    printk("Total logical processors: %d\n", list_get_elems(&lapic_list));
    printk("Total IO-APIC interrupt overrides: %d\n", list_get_elems(&ioapic_intr_overrides));
    sys_lapic_init();
    printk("Initialized local APIC :)\n");
    sys_ioapic_init();
    printk("Initialized IO APIC :)\n");
    ioapic_set_entry(ioapic_redirect_irq(0), 32);
    printk("APIC has now been enabled\n");
}
