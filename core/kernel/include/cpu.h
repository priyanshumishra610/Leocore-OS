#pragma once
#include "types.h"

static inline void cpu_vendor(char out[13]) {
	u32 ebx=0, ecx=0, edx=0;
	__asm__ volatile (
		"cpuid"
		: "=b"(ebx), "=c"(ecx), "=d"(edx)
		: "a"(0)
	);
	((u32*)out)[0] = ebx;
	((u32*)out)[1] = edx;
	((u32*)out)[2] = ecx;
	out[12] = '\0';
}
