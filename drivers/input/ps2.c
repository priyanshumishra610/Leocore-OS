#include <stdint.h>
#include "../../core/kernel/include/io.h"
#include "../../core/kernel/include/log.h"

#define KBD_DATA 0x60

void ps2_init(void) {
	// For now nothing; keyboard controller already enabled by BIOS/GRUB.
}

void ps2_handle_irq(void) {
	uint8_t sc = inb(KBD_DATA);
	(void)sc; // TODO: translate
	// Basic debug log (avoid excessive output):
	// log_info("kbd irq");
}
