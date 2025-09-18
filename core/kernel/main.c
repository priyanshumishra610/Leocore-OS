#include <stdint.h>

static volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static const int VGA_COLS = 80;
static const int VGA_ROWS = 25;

static int cursor_col = 0;
static int cursor_row = 0;

static uint16_t vga_entry(char c, uint8_t color) {
	return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_clear() {
	uint8_t color = 0x07; // light grey on black
	for (int y = 0; y < VGA_ROWS; y++) {
		for (int x = 0; x < VGA_COLS; x++) {
			VGA_MEMORY[y * VGA_COLS + x] = vga_entry(' ', color);
		}
	}
	cursor_col = 0;
	cursor_row = 0;
}

static void vga_putc(char c) {
	uint8_t color = 0x07;
	if (c == '\n') {
		cursor_col = 0;
		cursor_row++;
	} else {
		VGA_MEMORY[cursor_row * VGA_COLS + cursor_col] = vga_entry(c, color);
		cursor_col++;
		if (cursor_col >= VGA_COLS) {
			cursor_col = 0;
			cursor_row++;
		}
	}
	if (cursor_row >= VGA_ROWS) {
		cursor_row = 0;
	}
}

static void print(const char* s) {
	for (const char* p = s; *p; p++) {
		vga_putc(*p);
	}
}

void kernel_main() {
	vga_clear();
	print("LeoCore OS kernel loaded!\n");
	while (1) { }
}
