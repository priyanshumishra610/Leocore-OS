#include "../include/mem.h"

void kmalloc_init(void) {
	// For Phase 1: initialize bump allocator over a static range
	bump_init(0x100000, 0x180000); // 1MiB..1.5MiB as a placeholder
}

void* kmalloc(u32 size) {
	return bump_alloc(size, 8);
}
