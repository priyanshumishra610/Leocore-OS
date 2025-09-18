#include "include/vga.h"

static volatile u16* const VGA_MEM = (u16*)0xB8000;
static const int VGA_COLS = 80;
static const int VGA_ROWS = 25;
static int cursor_col = 0;
static int cursor_row = 0;
static u8 current_color = (VGA_LIGHT_GREY | (VGA_BLACK << 4));

static u16 vga_entry(char c, u8 color) { return (u16)c | ((u16)color << 8); }

void vga_set_color(vga_color_t fg, vga_color_t bg) { current_color = (u8)(fg | (bg << 4)); }

void vga_clear(void) {
	for (int y = 0; y < VGA_ROWS; y++) {
		for (int x = 0; x < VGA_COLS; x++) VGA_MEM[y*VGA_COLS + x] = vga_entry(' ', current_color);
	}
	cursor_col = 0; cursor_row = 0;
}

static void vga_newline(void) {
	cursor_col = 0;
	cursor_row++;
	if (cursor_row >= VGA_ROWS) {
		// scroll up by one line
		for (int y = 1; y < VGA_ROWS; y++) {
			for (int x = 0; x < VGA_COLS; x++) {
				VGA_MEM[(y-1)*VGA_COLS + x] = VGA_MEM[y*VGA_COLS + x];
			}
		}
		for (int x = 0; x < VGA_COLS; x++) VGA_MEM[(VGA_ROWS-1)*VGA_COLS + x] = vga_entry(' ', current_color);
		cursor_row = VGA_ROWS - 1;
	}
}

void vga_putc(char c) {
	if (c == '\n') { vga_newline(); return; }
	VGA_MEM[cursor_row*VGA_COLS + cursor_col] = vga_entry(c, current_color);
	cursor_col++;
	if (cursor_col >= VGA_COLS) vga_newline();
}

void vga_write(const char* s) { for (const char* p = s; *p; ++p) vga_putc(*p); }

void vga_init(void) { vga_set_color(VGA_LIGHT_GREY, VGA_BLACK); vga_clear(); }
