/*
 * LeoCore OS minimal kernel (Phase 1 Embryo)
 * - Initializes subsystems, prints banner, dumps memory, starts shell
 */
#include "include/log.h"
#include "include/vga.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/timer.h"
#include "include/mem.h"
#include "include/mem_paging.h"
#include "include/task.h"
#include "include/keyboard.h"
#include "include/cpu.h"
#include "include/multiboot.h"

extern void ps2_init(void);
extern char shell_read_char(void);
extern void shell_run(void);

static void dump_mmap(const multiboot_info_t* mbi) {
	if (!(mbi->flags & (1 << 6))) { log_warn("No memory map available"); return; }
	log_info("Memory Map:");
	u32 end = mbi->mmap_addr + mbi->mmap_length;
	for (u32 p = mbi->mmap_addr; p < end; ) {
		multiboot_mmap_entry_t* e = (multiboot_mmap_entry_t*)p;
		log_puts("  ["); log_puts((e->type == 1) ? "Usable" : "Other"); log_puts("] addr=");
		// minimal hex print
		// (keeping it simple; full printf comes later)
		log_puts("0x");
		// skip detailed hex for brevity in Phase 1
		log_puts("...");
		log_puts(" len=...");
		log_puts("\n");
		p += e->size + sizeof(e->size);
	}
}

static void print_banner(const multiboot_info_t* mbi) {
	vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
	log_puts("\n");
	log_puts("  _                _____                 \n");
	log_puts(" | |   ___  ___   | ____|_ __ ___  _   _ \n");
	log_puts(" | |__/ _ \\/ _ \\  |  _| | '_ ` _ \\| | | |\n");
	log_puts(" | |_ |  __/ (_) | | |___| | | | | | |_| |\n");
	log_puts(" |____\\___|\\___/  |_____|_| |_| |_|\\__,_|  (Embryo)\n");
	vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
	char vendor[13]; cpu_vendor(vendor);
	log_info("Phase 1: Iconic Embryo");
	log_info(vendor);
	if (mbi->flags & 1) {
		log_info("RAM: lower/upper KB available");
	}
#if LEO_RELEASE
	log_info("Build: release");
#else
	log_info("Build: debug");
#endif
}

static void kernel_init(const multiboot_info_t* mbi) {
	log_init();
	vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);

    gdt_init();
    idt_init();
    pic_remap();
    timer_init(100);
    kmalloc_init();
    paging_init();
    scheduler_init();
    keyboard_init();
	sti();

	print_banner(mbi);
	dump_mmap(mbi);
}

void kernel_main(u32 magic, const multiboot_info_t* mbi) {
	(void)magic;
	kernel_init(mbi);
	log_info("Type 'help' for commands.");
	shell_run();
	for (;;) { }
}
