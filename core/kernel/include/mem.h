#pragma once
#include "types.h"

void bump_init(u32 start, u32 end);
void* bump_alloc(u32 size, u32 align);

void kmalloc_init(void);
void* kmalloc(u32 size);
