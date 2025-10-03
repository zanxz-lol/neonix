#ifndef __X86_64_ACPI_H
#define __X86_64_ACPI_H

#include <naho/compiler.h>
#include <stdint.h>

struct acpi_table_entry {
    char signature[4];
    void (*init)(void *);
};

struct xsdp {
    char signature[8];
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t revision;
    uint32_t deprecated;

    uint32_t length;
    uint64_t xsdt_addr;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __packed;

struct acpi_sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __packed;

struct xsdt {
    struct acpi_sdt_header header;
    uint64_t pointers[];
} __packed;

struct generic_addr_struct {
    uint8_t addr_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t addr;
} __packed;

struct fadt {
    struct acpi_sdt_header header;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t deprecated;
    uint8_t preferred_power_profile;
    uint16_t sys_control_intr;
    uint32_t sys_management_port;
    uint8_t acpi_enable;
    uint8_t acpi_disable;
    uint8_t s4bios_req;
    uint8_t pstate_control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;

    uint8_t  deprecated2;
    uint32_t Flags;

    // 12 byte structure; see below for details
    struct generic_addr_struct reset_reg;

    uint8_t reset_value;
    uint8_t reserved[3];
  
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t xfirmware_ctrl;
    uint64_t xdsdt;

    struct generic_addr_struct X_PM1aEventBlock;
    struct generic_addr_struct X_PM1bEventBlock;
    struct generic_addr_struct X_PM1aControlBlock;
    struct generic_addr_struct X_PM1bControlBlock;
    struct generic_addr_struct X_PM2ControlBlock;
    struct generic_addr_struct X_PMTimerBlock;
    struct generic_addr_struct X_GPE0Block;
    struct generic_addr_struct X_GPE1Block;

} __packed;

struct acpi_entry_header {
    uint8_t type;
    uint8_t length;
} __packed;

struct madt_local_apic {
    struct acpi_entry_header header;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __packed;

struct madt_io_apic {
    struct acpi_entry_header header;
    uint8_t id;
    uint8_t reserved;
    uint32_t ioapic_addr;
    uint32_t gsi_base;
} __packed;

struct madt_io_apic_iso {
    struct acpi_entry_header header;
    uint8_t bus_src;
    uint8_t irq_src;
    uint32_t gsi;
    uint16_t flags;
} __packed;

struct madt_io_apic_nmi {
    struct acpi_entry_header header;
    uint8_t nmi_src;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gsi;
} __packed;

struct madt_local_apic_nmi {
    struct acpi_entry_header header;
    uint8_t processor_id;
    uint16_t flags;
    uint8_t lint;
} __packed;

struct madt_local_x2apic {
    struct acpi_entry_header header;
    uint16_t reserved;
    uint32_t proc_lx2apic_id;
    uint32_t flags;
    uint32_t acpi_id;
};

struct madt {
    struct acpi_sdt_header header;
    uint32_t lapic_addr;
    uint32_t flags;
} __packed;

void sys_acpi_enable(void * table); /* FACP */
int sys_acpi_init(void);

#endif
