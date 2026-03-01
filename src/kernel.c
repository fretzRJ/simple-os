#include "io.h"
#include "editor.h"
#include "snake.h"

static void read_line(char* buffer, int max_len) {
    int idx = 0;

    while (1) {
        char c = keyboard_read_char();

        if (c == '\n') {
            vga_putc('\n');
            break;
        }

        if (c == '\b') {
            if (idx > 0) {
                idx--;
                vga_putc('\b');
            }
            continue;
        }

        if (idx < max_len - 1 && c >= ' ' && c <= '~') {
            buffer[idx++] = c;
            vga_putc(c);
        }
    }

    buffer[idx] = '\0';
}

static void print_banner(void) {
    vga_set_color(15, 1);
    vga_clear();
    vga_puts("SimpleOS 32-bit\n");
    vga_puts("Runs on old x86 PCs via GRUB\n\n");
    vga_set_color(15, 0);
}

void kernel_main(void) {
    char cmd[64];

    print_banner();
    vga_puts("Commands: help, editor, snake, clear, about\n\n");

    while (1) {
        vga_puts("simple-os> ");
        read_line(cmd, sizeof(cmd));

        if (str_eq(cmd, "help")) {
            vga_puts("help   - show commands\n");
            vga_puts("editor - open text editor\n");
            vga_puts("snake  - play snake\n");
            vga_puts("clear  - clear screen\n");
            vga_puts("about  - about this OS\n");
        } else if (str_eq(cmd, "editor")) {
            run_text_editor();
            print_banner();
        } else if (str_eq(cmd, "snake")) {
            run_snake();
            print_banner();
        } else if (str_eq(cmd, "clear")) {
            vga_clear();
        } else if (str_eq(cmd, "about")) {
            vga_puts("SimpleOS is a tiny educational 32-bit kernel.\n");
            vga_puts("It has a shell, in-memory text editor, and snake game.\n");
        } else if (str_len(cmd) == 0) {
            continue;
        } else {
            vga_puts("Unknown command. Type help.\n");
        }
    }
}
