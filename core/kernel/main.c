/*
 * LeoCore OS minimal kernel (Phase 1 Advanced)
 * - Initializes logging (serial + VGA) and prints hello
 * - Prepares hooks for GDT/IDT/PIC/PIT and drivers
 */
#include "include/log.h"
#include "include/vga.h"

static void kernel_init(void) {
	log_init();
	vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
}

void kernel_main() {
	kernel_init();
	log_info("LeoCore OS kernel loaded!");
	for (;;) { }
}
