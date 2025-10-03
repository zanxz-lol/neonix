#include <naho/printk.h>
#include <naho/proc.h>
#include <x86_64/sched.h>
#include <x86_64/intr.h>
#include <x86_64/timer.h>
#include <x86_64/pmio.h>
#include <x86_64/apic.h>
#include <stdint.h>
#include <config.h>

#ifdef CONFIG_NAHO_FUNTIME
#include <x86_64/bad_apple.h>
#endif

#define PIT_FREQ_DIVISOR 1193180

static volatile uint64_t ticks;

static void i8253_configure_hz(uint16_t hz) {
    printk("Configuring i8253 PIT @ %dHz\n", hz);
    uint32_t divisor = PIT_FREQ_DIVISOR / hz;  
    pmio_write8(0x43, 0x36); 
    pmio_write8(0x40, divisor & 0xff);
    pmio_write8(0x40, divisor >> 8);
}

#ifdef CONFIG_NAHO_FUNTIME

static void play_sound(uint32_t freq) {
	uint32_t divisor;
	uint8_t tmp;

	divisor = PIT_FREQ_DIVISOR / freq;
	pmio_write8(0x43, 0xb6);
	pmio_write8(0x42, divisor);
	pmio_write8(0x42, divisor >> 8);

	tmp = pmio_read8(0x61);
	if (tmp != (tmp | 3)) {
		pmio_write8(0x61, tmp | 3);
	}
}

static void nosound(void) {
	uint8_t tmp = pmio_read8(0x61) & 0xFC;
	pmio_write8(0x61, tmp);
}

static void usleep(uint64_t us) {
    uint64_t end = ticks + us;
    while(end > ticks);
}

void arch_bad_apple(void) {
    size_t notes = sizeof(bad_apple) / sizeof(bad_apple[0]);
    printk("Total notes in music (including rests): %d\n", notes);
    for(size_t i = 0; i < notes; i++) {
        if (bad_apple[i].frequency == 0) {
            nosound();
        } else {
	        play_sound(bad_apple[i].frequency);
        }
	    usleep(bad_apple[i].duration);
    }
	nosound();
}

#endif

static void timer_irq(struct intr_stack_frame * regs) {
    ticks++;
    lapic_send_eoi();
}

void sys_i8253_init(void) {
    i8253_configure_hz(1000);
    intr_add_handler(32, timer_irq);
}
