#pragma once
#include "types.h"

static inline void outb(u16 port, u8 val) {
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
	u8 ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void io_wait(void) {
	/* Port 0x80 is traditionally used for 'checkpoints' */
	__asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}
