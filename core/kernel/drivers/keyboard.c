#include "../include/types.h"
#include "../include/log.h"

extern void ps2_init(void);

void keyboard_init(void) {
	ps2_init();
	log_info("keyboard: ps/2 init");
}

int keyboard_read_char(void) {
	// Phase 2: wire to PS/2 scancode decoder. For now return no data.
	return -1;
}


