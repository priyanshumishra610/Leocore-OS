#pragma once
#include "types.h"

typedef struct {
	u16 offset_low;
	u16 selector;
	u8 zero;
	u8 type_attr;
	u16 offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	u16 limit;
	u32 base;
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);
void irq_install(void);
