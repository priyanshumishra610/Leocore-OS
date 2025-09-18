#include "../include/mem.h"

static u32 bump_ptr = 0;
static u32 bump_end = 0;

void bump_init(u32 start, u32 end) { bump_ptr = start; bump_end = end; }

void* bump_alloc(u32 size, u32 align) {
	if (align && (bump_ptr & (align - 1))) bump_ptr = (bump_ptr + align - 1) & ~(align - 1);
	if (bump_ptr + size > bump_end) return 0;
	void* p = (void*)bump_ptr;
	bump_ptr += size;
	return p;
}
