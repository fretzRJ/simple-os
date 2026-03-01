#ifndef IO_H
#define IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void vga_clear(void);
void vga_putc(char c);
void vga_puts(const char* s);
void vga_set_color(uint8_t fg, uint8_t bg);
void vga_move_cursor(uint8_t row, uint8_t col);
void vga_draw_char(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg);

char keyboard_read_char(void);

int str_eq(const char* a, const char* b);
int str_len(const char* s);
void str_copy(char* dst, const char* src);

#endif
