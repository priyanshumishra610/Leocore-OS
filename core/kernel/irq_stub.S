.intel_syntax noprefix
.global irq_common_stub
.global irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	push eax
	call irq_handler_c
	add esp, 4
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 4
	iret

#define IRQ_STUB(n) \
.global irq##n; \
irq##n: \
	push dword n; \
	jmp irq_common_stub

IRQ_STUB(0)
IRQ_STUB(1)
IRQ_STUB(2)
IRQ_STUB(3)
IRQ_STUB(4)
IRQ_STUB(5)
IRQ_STUB(6)
IRQ_STUB(7)
IRQ_STUB(8)
IRQ_STUB(9)
IRQ_STUB(10)
IRQ_STUB(11)
IRQ_STUB(12)
IRQ_STUB(13)
IRQ_STUB(14)
IRQ_STUB(15)
