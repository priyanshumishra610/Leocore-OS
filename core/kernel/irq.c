#include "include/types.h"
#include "include/io.h"
#include "include/log.h"
#include "include/task.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI      0x20

extern void timer_tick(void);
extern void ps2_handle_irq(void);

void irq_handler_c(u32 irq_num) {
	if (irq_num >= 8) outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);

    if (irq_num == 0) {
        timer_tick();
        scheduler_tick();
	} else if (irq_num == 1) {
		ps2_handle_irq();
	}
}
