#include <naho/init.h>
#include <naho/kernel.h>
#include <naho/printk.h>
#include <naho/console.h>
#include <naho/errno.h>
#include <lib/limine.h>
#include <lib/common.h>
#include <lib/string.h>
/* the console font */
#include <font/vga8x16.h>

#include <config.h>

#ifdef CONFIG_NAHO_VIDEO

static struct console_font * font;
static struct console console = {
    .name = "limine_console"
};

static uintptr_t fb_addr = 0;
static uintptr_t fb_width = 0;
static uintptr_t fb_height = 0;
static uintptr_t fb_pitch = 0;

static void limine_fbcon_putpx(size_t x, size_t y, uint32_t rgb) {
    uint32_t * buf = (uint32_t *)fb_addr;
    uint32_t offset = y * (fb_pitch / 4) + x;
    buf[offset] = rgb;
}

static inline void fbcon_scroll(void) {
    memcpy((void *)fb_addr, (void *)fb_addr + fb_pitch, console.data.cols * fb_pitch);
    memset((void *)fb_addr + (console.data.cols * fb_pitch), 0, fb_pitch);
}

static void limine_fbcon_put_glyph(const char ch) {
    if (ch > font->size) {
        return;
    }
    uint8_t * glyph = &font->data[ch * font->height];
    size_t x = GET_CONSOLE_X(console.data.position) * font->width;
    size_t y = GET_CONSOLE_Y(console.data.position) * font->height;
    
    for (size_t dy = 0; dy < font->height; dy++) {
        for (size_t dx = 0; dx < font->width; dx++) {
            bool foreground = ((glyph[dy] >> (7 - dx)) & 1);
            if (foreground) {
                limine_fbcon_putpx(x + dx, y + dy, 0xaaaaaa);
            } else {
                limine_fbcon_putpx(x + dx, y + dy, 0x000000);
            }
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
    fb_addr = (uintptr_t)boot_info.fb_list[0]->address;
    fb_width = boot_info.fb_list[0]->width;
    fb_height = boot_info.fb_list[0]->height;
    fb_pitch = boot_info.fb_list[0]->pitch;
    console.data.font = font_vga8x16;
    console.data.rows = boot_info.fb_list[0]->width / font_vga8x16.width;
    console.data.cols = boot_info.fb_list[0]->height / font_vga8x16.height;
    console.data.attribtutes |= CONSOLE_ENABLED;
    console.data.has_color = true;
    console.data.position = 0;
    console.data.ops.write = limine_fbcon_write;
    font = &console.data.font;
    int rc = register_console(&console);
    if (IS_ERROR(rc) == true) {
        printk("Failed to register console: %s\n", errno2str(rc));
    }
    return rc;
}

#endif
