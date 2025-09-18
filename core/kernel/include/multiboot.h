#pragma once
#include "types.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

typedef struct multiboot_mmap_entry {
	u32 size;
	u64 addr;
	u64 len;
	u32 type;
} __attribute__((packed)) multiboot_mmap_entry_t;

typedef struct multiboot_info {
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	u32 syms[4];
	u32 mmap_length;
	u32 mmap_addr;
	/* truncated */
} multiboot_info_t;
