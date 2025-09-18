.intel_syntax noprefix
.global outb
.global inb
.global io_wait

outb:
	push ebp
	mov ebp, esp
	mov dx, [ebp+8]
	mov al, byte ptr [ebp+12]
	out dx, al
	pop ebp
	ret

inb:
	push ebp
	mov ebp, esp
	mov dx, [ebp+8]
	in al, dx
	movzx eax, al
	pop ebp
	ret

io_wait:
	push ebp
	mov ebp, esp
	mov dx, 0x80
	xor eax, eax
	out dx, al
	pop ebp
	ret
