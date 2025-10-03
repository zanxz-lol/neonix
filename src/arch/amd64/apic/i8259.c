#include <naho/printk.h>
#include <x86_64/pmio.h>

#define PIC_MASTER_COMMAND_PORT 0x0020
#define PIC_MASTER_DATA_PORT    0x0021
#define PIC_SLAVE_COMMAND_PORT  0x00a0
#define PIC_SLAVE_DATA_PORT     0x00a1

#define PIC_EOI	0x20
#define ICW1_ICW4	0x01
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT	0x10

#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

static void pic_master_send_cmd(uint8_t cmd) {
    pmio_write8(PIC_MASTER_COMMAND_PORT, cmd);
}

static void pic_master_send_dat(uint8_t data) {
    pmio_write8(PIC_MASTER_DATA_PORT, data);
}

static void pic_slave_send_cmd(uint8_t cmd) {
    pmio_write8(PIC_SLAVE_COMMAND_PORT, cmd);
}

static void pic_slave_send_dat(uint8_t data) {
    pmio_write8(PIC_SLAVE_DATA_PORT, data);
}

uint8_t read_pic_master_data(void) {
    return pmio_read8(PIC_MASTER_DATA_PORT);
}

uint8_t read_pic_slave_data(void) {
    return pmio_read8(PIC_SLAVE_DATA_PORT);
}

void sys_disable_legacy_i8259(void) {
    uint8_t a1, a2;

    a1 = read_pic_slave_data();
    a2 = read_pic_master_data();

    pic_master_send_cmd(ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    pic_slave_send_cmd(ICW1_INIT | ICW1_ICW4);
    pic_master_send_dat(0x20);                 // ICW2: Master PIC vector offset
    pic_slave_send_dat(0x28);                 // ICW2: Slave PIC vector offset
    pic_master_send_dat(4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    pic_slave_send_dat(2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    pic_master_send_dat(ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    pic_slave_send_dat(ICW4_8086);
    pic_master_send_dat(0xff);   // restore saved masks.
    pic_slave_send_dat(0xff);
    printk("Legacy i8259 PIC has been disabled.\n");
}
