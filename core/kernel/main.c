/*
 * LeoCore OS minimal kernel (Phase 1 Advanced)
 * - Initializes logging (serial + VGA) and prints hello
 * - Sets up GDT, IDT, PIC, PIT, memory alloc, and keyboard stub
 */
#include "include/log.h"
#include "include/vga.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/timer.h"
#include "include/mem.h"

extern void ps2_init(void);

static void kernel_init(void) {
	log_init();
	vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);

	gdt_init();
	idt_init();
	pic_remap();
	timer_init(100);
	kmalloc_init();
	ps2_init();
	sti();
}

void kernel_main() {
	kernel_init();
	log_info("LeoCore OS kernel loaded!");
	for (;;) { }
}
