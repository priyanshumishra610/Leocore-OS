#include "../core/kernel/include/types.h"
#include "../core/kernel/include/io.h"
#include "../core/kernel/include/log.h"

#define PIT_CH0      0x40
#define PIT_CMD      0x43
#define PIT_SET      1193180

static volatile u32 ticks = 0;

void timer_init(u32 freq_hz) {
	u32 divisor = (freq_hz ? (PIT_SET / freq_hz) : 0);
	outb(PIT_CMD, 0x36);
	outb(PIT_CH0, (u8)(divisor & 0xFF));
	outb(PIT_CH0, (u8)((divisor >> 8) & 0xFF));
}

void timer_tick(void) {
	if (++ticks % 100 == 0) log_puts(".");
}

