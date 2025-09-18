#include "include/panic.h"
#include "include/log.h"

extern void cli_asm(void);
extern void hlt_asm(void);

void panic(const char* msg) {
	cli_asm();
	log_puts("\n[ PANIC ] ");
	log_puts(msg);
	log_puts("\nSystem halted.\n");
	for (;;) hlt_asm();
}
