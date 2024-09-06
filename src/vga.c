#include "vga.h"
#include "utils/util.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT  0x3D5

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_setcursorposition(0, 0);  // Initialize cursor position
}

void terminal_setcursorposition(size_t x, size_t y) {
    uint16_t position = y * VGA_WIDTH + x;
    
    // Send the high byte of the position
    outportb(VGA_INDEX_PORT, 0x0E);
    outportb(VGA_DATA_PORT, (position >> 8) & 0xFF);
    
    // Send the low byte of the position
    outportb(VGA_INDEX_PORT, 0x0F);
    outportb(VGA_DATA_PORT, position & 0xFF);
}

void terminal_getcursorposition(int *x, int *y) {
    uint16_t position;
    
    // Get the high byte of the position
    outportb(VGA_INDEX_PORT, 0x0E);
    position = inportb(VGA_DATA_PORT) << 8;
    
    // Get the low byte of the position
    outportb(VGA_INDEX_PORT, 0x0F);
    position |= inportb(VGA_DATA_PORT);
    
    *x = position % VGA_WIDTH;
    *y = position / VGA_WIDTH;
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_backspace(void) {
    int x, y;
    terminal_getcursorposition(&x, &y);
    if (x > 0) {
        x--;
    } else if (y > 0) {
        x = VGA_WIDTH - 1;
        y--;
    }
    terminal_putentryat(' ', terminal_color, x, y);
    terminal_setcursorposition(x, y);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    int x, y;
    terminal_getcursorposition(&x, &y);
    
    terminal_putentryat(c, terminal_color, x, y);
    if (++x == VGA_WIDTH) {
        x = 0;
        if (++y == VGA_HEIGHT) {
            y = VGA_HEIGHT - 1;
            scroll_up();  // Scroll up if at bottom
        }
    }

    terminal_setcursorposition(x, y);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            int x, y;
            terminal_getcursorposition(&x, &y);
            x = 0;
            if (++y == VGA_HEIGHT) {
                y = VGA_HEIGHT - 1;
                scroll_up();  // Scroll up if at bottom
            }
            terminal_setcursorposition(x, y);
            continue;
        }

        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void scroll_up(void) {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            char c = terminal_getchar_at(x, y);
            uint8_t color = terminal_getcolor_at(x, y);
            terminal_putentryat(c, color, x, y - 1);
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY), x, VGA_HEIGHT - 1);
    }

    int x, y;
    terminal_getcursorposition(&x, &y);
    if (y >= VGA_HEIGHT) {
        y = VGA_HEIGHT - 1;
        terminal_setcursorposition(x, y);
    }
}

char terminal_getchar_at(size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return ' ';
    }
    const size_t index = y * VGA_WIDTH + x;
    return terminal_buffer[index] & 0xFF;
}

uint8_t terminal_getcolor_at(size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return 0;
    }
    const size_t index = y * VGA_WIDTH + x;
    return terminal_buffer[index] >> 8;
}
