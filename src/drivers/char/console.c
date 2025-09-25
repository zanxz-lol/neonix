#include <neonix/console.h>
#include <neonix/printk.h>
#include <neonix/errno.h>
#include <lib/common.h>
#include <lib/string.h>
#include <lib/bool.h>

static struct console * consoles[MAX_NUM_CONSOLES];
static int num_consoles = 0;
static int default_console = -1;

bool is_console_available(void) {
    if (default_console < 0) return false;
    return (consoles[default_console] != NULL) ? true : false;   
}

int register_console(struct console * con) {
    if (con == NULL) {
        return -EBADADDR;
    }
    if (num_consoles < 1) {
        default_console = num_consoles++;
    }
    consoles[default_console] = con;
    con->data.num = default_console;
    printk_copy_buffer_to_console();
    return 0;
}

void console_puts(const char * string) {
    for(size_t i = 0; string[i] != 0; i++) {
        console_putc(string[i]);
    }
}

void console_putc(const char ch) {
    if (is_console_available() == false) {
        return;
    }
    if (consoles[default_console]->data.ops.write == NULL) {
        return;
    }
    size_t pos = consoles[default_console]->data.position;
    if (ch < 32) {
        switch (ch) {
            case '\n':
                pos = SET_CONSOLE_POSITION(0, GET_CONSOLE_Y(pos) + 1);
                break;
            case '\t':
                pos = SET_CONSOLE_POSITION(GET_CONSOLE_X(pos) + 4, GET_CONSOLE_Y(pos));
                break;
            default:
                break;
        }
        consoles[default_console]->data.position = pos;
        return;
    }
    consoles[default_console]->data.ops.write((uint8_t *)&ch, 1);
    pos = SET_CONSOLE_POSITION(GET_CONSOLE_X(pos) + 1, GET_CONSOLE_Y(pos));
    consoles[default_console]->data.position = pos;
}
