#include "include/types.h"
#include "include/idt.h"

extern void idt_load(u32);
extern void irq0(); extern void irq1(); extern void irq2(); extern void irq3(); extern void irq4(); extern void irq5(); extern void irq6(); extern void irq7(); extern void irq8(); extern void irq9(); extern void irq10(); extern void irq11(); extern void irq12(); extern void irq13(); extern void irq14(); extern void irq15();

static idt_entry_t idt[256];
static idt_ptr_t idtp;

static void idt_set_gate(int num, u32 base, u16 sel, u8 flags) {
	idt[num].offset_low = base & 0xFFFF;
	idt[num].selector = sel;
	idt[num].zero = 0;
	idt[num].type_attr = flags;
	idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(void) {
	idtp.limit = sizeof(idt) - 1;
	idtp.base = (u32)&idt;
	for (int i = 0; i < 256; ++i) idt_set_gate(i, 0, 0, 0);
	idt_set_gate(32+0, (u32)irq0, 0x08, 0x8E);
	idt_set_gate(32+1, (u32)irq1, 0x08, 0x8E);
	idt_set_gate(32+2, (u32)irq2, 0x08, 0x8E);
	idt_set_gate(32+3, (u32)irq3, 0x08, 0x8E);
	idt_set_gate(32+4, (u32)irq4, 0x08, 0x8E);
	idt_set_gate(32+5, (u32)irq5, 0x08, 0x8E);
	idt_set_gate(32+6, (u32)irq6, 0x08, 0x8E);
	idt_set_gate(32+7, (u32)irq7, 0x08, 0x8E);
	idt_set_gate(32+8, (u32)irq8, 0x08, 0x8E);
	idt_set_gate(32+9, (u32)irq9, 0x08, 0x8E);
	idt_set_gate(32+10, (u32)irq10, 0x08, 0x8E);
	idt_set_gate(32+11, (u32)irq11, 0x08, 0x8E);
	idt_set_gate(32+12, (u32)irq12, 0x08, 0x8E);
	idt_set_gate(32+13, (u32)irq13, 0x08, 0x8E);
	idt_set_gate(32+14, (u32)irq14, 0x08, 0x8E);
	idt_set_gate(32+15, (u32)irq15, 0x08, 0x8E);
	idt_load((u32)&idtp);
}
