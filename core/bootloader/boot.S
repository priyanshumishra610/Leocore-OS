/*
 * LeoCore OS Boot Stub (Multiboot v1)
 * - Provides a Multiboot header for GRUB to load this kernel at 32-bit protected mode.
 * - Transfers control to C entry (kernel_main).
 */

.set MULTIBOOT_MAGIC, 0x1BADB002
.set MULTIBOOT_FLAGS, 0x00010003  /* page-aligned modules + memory info */
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

/* Multiboot header placed in a .multiboot section so GRUB can find it */
.section .multiboot
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

/* Code entry point. GRUB jumps here. */
.section .text
.global _start
_start:
	/* Call the C kernel entry. GRUB sets up a 32-bit environment. */
	call kernel_main

	/* If kernel_main returns, halt the CPU. */
	hlt
