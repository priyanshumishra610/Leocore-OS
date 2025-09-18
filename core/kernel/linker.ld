ENTRY(_start)

SECTIONS
{
	. = 1M; /* Load at 1MiB */

	.text : ALIGN(4K)
	{
		*(.multiboot)
		*(.text*)
	}

	.rodata : ALIGN(4K) { *(.rodata*) }
	.data : ALIGN(4K) { *(.data*) }
	.bss : ALIGN(4K) { *(.bss*) }
}
