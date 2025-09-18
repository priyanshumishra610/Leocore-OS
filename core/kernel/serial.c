#include "include/serial.h"
#include "include/io.h"

#define COM1 0x3F8

void serial_init(void) {
	outb(COM1 + 1, 0x00); // disable interrupts
	outb(COM1 + 3, 0x80); // enable DLAB
	outb(COM1 + 0, 0x03); // 38400 baud (divisor low)
	outb(COM1 + 1, 0x00); // divisor high
	outb(COM1 + 3, 0x03); // 8N1, disable DLAB
	outb(COM1 + 2, 0xC7); // enable FIFO, clear them, 14-byte threshold
	outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static int serial_can_tx(void) { return inb(COM1 + 5) & 0x20; }

void serial_putc(char c) {
	while (!serial_can_tx()) { }
	outb(COM1, (u8)c);
}

void serial_write(const char* s) { for (const char* p = s; *p; ++p) serial_putc(*p); }
