#include "include/types.h"
#include "include/io.h"
#include "include/log.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI      0x20

void irq_handler_c(u32 irq_num) {
	// Send EOI to PICs
	if (irq_num >= 8) outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);

	if (irq_num == 0) {
		// timer tick
	} else if (irq_num == 1) {
		// keyboard
	}
}
