/*
 * LeoCore OS Boot Stub (Multiboot v1)
 * - Provides a Multiboot header for GRUB (32-bit protected mode).
 * - Transfers control to C entry kernel_main(magic, mbi).
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

/* Code entry point. GRUB jumps here with EAX=magic and EBX=mbi. */
.section .text
.global _start
_start:
	/* Push arguments (cdecl): push mbi, then magic */
	push %ebx
	push %eax
	call kernel_main
	add $8, %esp

	/* If kernel_main returns, halt the CPU. */
	hlt
