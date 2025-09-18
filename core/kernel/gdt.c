#include "include/types.h"
#include "include/gdt.h"

struct __attribute__((packed)) gdt_entry { u16 limit_low; u16 base_low; u8 base_mid; u8 access; u8 gran; u8 base_hi; };
struct __attribute__((packed)) gdt_ptr { u16 limit; u32 base; };

extern void gdt_flush(u32 ptr);

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set(int idx, u32 base, u32 limit, u8 access, u8 gran) {
	gdt[idx].limit_low = (limit & 0xFFFF);
	gdt[idx].base_low  = (base & 0xFFFF);
	gdt[idx].base_mid  = (base >> 16) & 0xFF;
	gdt[idx].access    = access;
	gdt[idx].gran      = ((limit >> 16) & 0x0F) | (gran & 0xF0);
	gdt[idx].base_hi   = (base >> 24) & 0xFF;
}

void gdt_init(void) {
	gp.limit = sizeof(gdt) - 1;
	gp.base  = (u32)&gdt;
	gdt_set(0, 0, 0, 0, 0);
	gdt_set(1, 0, 0xFFFFF, 0x9A, 0xCF); // code
	gdt_set(2, 0, 0xFFFFF, 0x92, 0xCF); // data
	gdt_flush((u32)&gp);
}
