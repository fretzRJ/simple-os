#include "snake.h"
#include "io.h"

#define WIDTH 50
#define HEIGHT 20
#define MAX_LEN 128

typedef struct {
    int x;
    int y;
} Point;

static uint32_t rng_state = 0x12345678;

static uint32_t rand_next(void) {
    rng_state = rng_state * 1664525 + 1013904223;
    return rng_state;
}

static void draw_border(void) {
    for (int x = 0; x < WIDTH; x++) {
        vga_draw_char(2, x, '#', 10, 0);
        vga_draw_char(HEIGHT + 3, x, '#', 10, 0);
    }
    for (int y = 0; y < HEIGHT; y++) {
        vga_draw_char(y + 3, 0, '#', 10, 0);
        vga_draw_char(y + 3, WIDTH - 1, '#', 10, 0);
    }
}

static void busy_delay(void) {
    for (volatile int i = 0; i < 2500000; i++) {
    }
}

void run_snake(void) {
    Point snake[MAX_LEN];
    int len = 4;
    int dx = 1;
    int dy = 0;

    for (int i = 0; i < len; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }

    Point food = {20, 10};

    vga_clear();
    vga_puts("Snake: WASD to move, ESC to quit\n");
    draw_border();

    while (1) {
        char key = 0;
        uint8_t status = inb(0x64);
        if (status & 1) {
            char c = keyboard_read_char();
            key = c;
        }

        if (key == 27) {
            break;
        } else if (key == 'w' && dy != 1) {
            dx = 0; dy = -1;
        } else if (key == 's' && dy != -1) {
            dx = 0; dy = 1;
        } else if (key == 'a' && dx != 1) {
            dx = -1; dy = 0;
        } else if (key == 'd' && dx != -1) {
            dx = 1; dy = 0;
        }

        Point head = { snake[0].x + dx, snake[0].y + dy };

        if (head.x <= 0 || head.x >= WIDTH - 1 || head.y <= 0 || head.y >= HEIGHT - 1) {
            break;
        }

        for (int i = 0; i < len; i++) {
            if (snake[i].x == head.x && snake[i].y == head.y) {
                vga_puts("\nGame Over!\n");
                for (volatile int d = 0; d < 10000000; d++) {}
                vga_clear();
                return;
            }
        }

        for (int i = len; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = head;

        if (head.x == food.x && head.y == food.y) {
            if (len < MAX_LEN - 1) {
                len++;
            }
            food.x = (int)(rand_next() % (WIDTH - 2)) + 1;
            food.y = (int)(rand_next() % (HEIGHT - 2)) + 1;
        }

        Point tail = snake[len];
        vga_draw_char((uint8_t)(tail.y + 3), (uint8_t)tail.x, ' ', 15, 0);
        vga_draw_char((uint8_t)(food.y + 3), (uint8_t)food.x, '*', 12, 0);

        for (int i = 0; i < len; i++) {
            vga_draw_char((uint8_t)(snake[i].y + 3), (uint8_t)snake[i].x, i == 0 ? '@' : 'o', 14, 0);
        }

        busy_delay();
    }

    vga_clear();
}
