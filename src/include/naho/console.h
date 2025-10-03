#ifndef __NAHO_CONSOLE_H
#define __NAHO_CONSOLE_H

#include <lib/bool.h>
#include <stddef.h>
#include <stdint.h>

#include <config.h>

#define MAX_NUM_CONSOLES 32

/* console flags */
#define CONSOLE_ENABLED (1 << 0)
/* console positioning */
#define SET_CONSOLE_POSITION(x, y) ((((x) & 0xffff) << 16) | ((y) & 0xffff))
#define GET_CONSOLE_X(pos) (((pos) >> 16) & 0xffff)
#define GET_CONSOLE_Y(pos) ((pos) & 0xffff)

struct console_operations {
    void (*write)(const uint8_t * buffer, size_t size);
};

struct console_font {
    size_t width, height;
    uint8_t * data;
    size_t size;
};

struct console_data {
    unsigned int num;
    size_t rows, cols;
    uint8_t attribtutes;
    uint8_t default_colors;
    size_t position;
    bool has_color;
    struct console_operations ops;
    struct console_font font;
};

struct console {
    char name[32];
    struct console_data data;
};

bool is_console_available(void);
int register_console(struct console * con);
void console_puts(const char * string);
void console_putc(const char ch);

#ifdef CONFIG_NAHO_VIDEO
int limine_fbcon_init(void);
#endif

#endif
