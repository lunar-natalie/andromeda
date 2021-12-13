#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_BUFFER = (uint16_t*) 0xb8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

static inline void terminal_setdata(unsigned char c, uint8_t color, size_t x, size_t y)
{
	terminal_buffer[vga_index(y, VGA_WIDTH, x)] = vga_entry(c, color);
}

void terminal_init(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_FG, VGA_BG);
	terminal_buffer = VGA_BUFFER;
	/* fill each column in each row with null characters */
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = vga_index(y, VGA_WIDTH, x);
			terminal_buffer[index] = vga_entry(VGA_NULL, terminal_color);
		}
	}
}

void terminal_putc(unsigned char c)
{
	if (c == '\n') {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	} else {
		terminal_setdata(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
}

void terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putc((unsigned char) data[i]);
	return;
}

void terminal_puts(const char *str)
{
	terminal_write(str, strlen(str));
	return;
}