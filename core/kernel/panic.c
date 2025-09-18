#include "include/panic.h"
#include "include/log.h"

static inline void cli(void) { __asm__ volatile ("cli"); }
static inline void hlt(void) { __asm__ volatile ("hlt"); }

void panic(const char* msg) {
	cli();
	log_puts("\n[ PANIC ] ");
	log_puts(msg);
	log_puts("\nSystem halted.\n");
	for (;;) hlt();
}
