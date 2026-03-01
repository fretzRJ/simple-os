#include "editor.h"
#include "io.h"

#define EDIT_ROWS 20
#define EDIT_COLS 78

void run_text_editor(void) {
    char buffer[EDIT_ROWS][EDIT_COLS];
    for (int r = 0; r < EDIT_ROWS; r++) {
        for (int c = 0; c < EDIT_COLS; c++) {
            buffer[r][c] = ' ';
        }
    }

    int row = 0;
    int col = 0;

    vga_clear();
    vga_puts("Mini Text Editor (RAM only)\n");
    vga_puts("Type text. Enter=new line, Backspace=delete, ESC=exit.\n\n");

    while (1) {
        vga_move_cursor((uint8_t)(row + 3), (uint8_t)col);
        char ch = keyboard_read_char();

        if (ch == 27) {
            break;
        }

        if (ch == '\n') {
            if (row < EDIT_ROWS - 1) {
                row++;
                col = 0;
                vga_putc('\n');
            }
            continue;
        }

        if (ch == '\b') {
            if (col > 0) {
                col--;
                buffer[row][col] = ' ';
                vga_draw_char((uint8_t)(row + 3), (uint8_t)col, ' ', 15, 0);
            }
            continue;
        }

        if (ch >= ' ' && ch <= '~') {
            if (col < EDIT_COLS) {
                buffer[row][col] = ch;
                vga_draw_char((uint8_t)(row + 3), (uint8_t)col, ch, 15, 0);
                col++;
            }
        }
    }

    int used_cells = 0;
    for (int r = 0; r < EDIT_ROWS; r++) {
        for (int c = 0; c < EDIT_COLS; c++) {
            if (buffer[r][c] != ' ') {
                used_cells++;
            }
        }
    }

    (void)used_cells;
    vga_clear();
}
