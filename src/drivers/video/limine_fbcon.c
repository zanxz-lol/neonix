#include <neonix/errno.h>
#include <neonix/init.h>
#include <neonix/kernel.h>
#include <neonix/printk.h>
#include <neonix/console.h>
#include <lib/limine.h>
#include <lib/common.h>
#include <lib/string.h>
/* the console font */
#include <font/vga8x16.h>

static struct limine_framebuffer * fb = NULL;
static struct console_font * font;
static struct console console = {
    .name = "limine_console"
};

static void limine_fbcon_putpx(size_t x, size_t y, uint32_t rgb) {
    uint32_t * buf = (uint32_t *)fb->address;
    uint32_t offset = y * (fb->pitch / 4) + x;
    buf[offset] = rgb;
}

static inline void fbcon_scroll(void) {
    memcpy(fb->address, fb->address + fb->pitch, console.data.cols * fb->pitch);
    memset(fb->address + (console.data.cols * fb->pitch), 0, fb->pitch);
}

static void limine_fbcon_put_glyph(const char ch) {
    if (ch > font->size) {
        return;
    }
    uint8_t * glyph = &font->data[ch * font->height];
    size_t x = GET_CONSOLE_X(console.data.position) * font->width;
    size_t y = GET_CONSOLE_Y(console.data.position) * font->height;
    for (uint32_t dy = 0; dy < font->height; dy++) {
        for (uint32_t dx = 0; dx < font->width; dx++) {
            bool color = (glyph[dy] >> (7 - dx)) & 1;
            limine_fbcon_putpx(x + dx, y + dy, color ? 0xaaaaaa : 0x000000);
        }
    }
    if(GET_CONSOLE_X(console.data.position) > (console.data.rows - 1)) {
        SET_CONSOLE_POSITION(0, GET_CONSOLE_Y(console.data.position) + 1);
    }
    if (GET_CONSOLE_Y(console.data.position) > (console.data.cols - 1)) {
        fbcon_scroll();
    }
}

static void limine_fbcon_write(const uint8_t * buffer, size_t size) {
    for(size_t i = 0; i < size; i++) {
        limine_fbcon_put_glyph(buffer[i]);
    }
}

int limine_fbcon_init(void) {
    fb = boot_info.fb_list[0];
    console.data.font = font_vga8x16;
    console.data.rows = fb->width / font_vga8x16.width;
    console.data.cols = fb->height / font_vga8x16.height;
    console.data.attribtutes |= CONSOLE_ENABLED;
    console.data.has_color = true;
    console.data.position = 0;
    console.data.ops.write = limine_fbcon_write;
    font = &console.data.font;
    printk("Switching printk output to framebuffer..\n");
    return register_console(&console);
}
