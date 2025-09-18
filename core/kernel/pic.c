#include "include/io.h"
#include "include/pic.h"

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define ICW1_INIT       0x10
#define ICW1_ICW4       0x01
#define ICW4_8086       0x01

void pic_remap(void) {
	u8 a1 = inb(PIC1_DATA);
	u8 a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, 0x20);
	io_wait();
	outb(PIC2_DATA, 0x28);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void sti(void) { __asm__ volatile ("sti"); }
void cli(void) { __asm__ volatile ("cli"); }
