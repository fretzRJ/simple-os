#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;
static uint8_t color = 0x0F;

static uint16_t vga_entry(char c, uint8_t color_code) {
    return (uint16_t)c | ((uint16_t)color_code << 8);
}

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    color = (bg << 4) | (fg & 0x0F);
}

void vga_move_cursor(uint8_t row, uint8_t col) {
    cursor_row = row;
    cursor_col = col;
}

static void scroll_if_needed(void) {
    if (cursor_row < VGA_HEIGHT) {
        return;
    }

    for (int r = 1; r < VGA_HEIGHT; r++) {
        for (int c = 0; c < VGA_WIDTH; c++) {
            VGA_MEMORY[(r - 1) * VGA_WIDTH + c] = VGA_MEMORY[r * VGA_WIDTH + c];
        }
    }
    for (int c = 0; c < VGA_WIDTH; c++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + c] = vga_entry(' ', color);
    }

    cursor_row = VGA_HEIGHT - 1;
}

void vga_clear(void) {
    for (int r = 0; r < VGA_HEIGHT; r++) {
        for (int c = 0; c < VGA_WIDTH; c++) {
            VGA_MEMORY[r * VGA_WIDTH + c] = vga_entry(' ', color);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_draw_char(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint8_t clr = (bg << 4) | (fg & 0x0F);
    VGA_MEMORY[row * VGA_WIDTH + col] = vga_entry(c, clr);
}

void vga_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
        return;
    }

    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(' ', color);
        }
        return;
    }

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, color);
    cursor_col++;

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
    }
}

void vga_puts(const char* s) {
    while (*s) {
        vga_putc(*s++);
    }
}

static const char scan_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,  '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0
};

char keyboard_read_char(void) {
    while (1) {
        uint8_t status = inb(0x64);
        if ((status & 1) == 0) {
            continue;
        }

        uint8_t scancode = inb(0x60);
        if (scancode & 0x80) {
            continue;
        }

        if (scancode < sizeof(scan_to_ascii) && scan_to_ascii[scancode]) {
            return scan_to_ascii[scancode];
        }
    }
}

int str_eq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

int str_len(const char* s) {
    int len = 0;
    while (s[len]) {
        len++;
    }
    return len;
}

void str_copy(char* dst, const char* src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
}
