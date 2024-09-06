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

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_getcursorposition(int *x, int *y) {
    *x = terminal_column;
    *y = terminal_row;
}

void terminal_backspace() {
	terminal_column--;
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	terminal_setcursorposition(terminal_column, terminal_row);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	terminal_setcursorposition(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n'){
            terminal_column = 0;
            terminal_row++;
            if (terminal_row == VGA_HEIGHT)
                terminal_row = 0;
			
			terminal_setcursorposition(terminal_column, terminal_row);
            continue;
        }

		terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}