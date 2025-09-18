.att_syntax
.code32

.globl irq_common_stub
.globl irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

irq_common_stub:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi
	pushl 28(%esp)
	call irq_handler_c
	add $4, %esp
	popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax
	add $4, %esp
	iret

.globl irq0
irq0:
	pushl $0
	jmp irq_common_stub
.globl irq1
irq1:
	pushl $1
	jmp irq_common_stub
.globl irq2
irq2:
	pushl $2
	jmp irq_common_stub
.globl irq3
irq3:
	pushl $3
	jmp irq_common_stub
.globl irq4
irq4:
	pushl $4
	jmp irq_common_stub
.globl irq5
irq5:
	pushl $5
	jmp irq_common_stub
.globl irq6
irq6:
	pushl $6
	jmp irq_common_stub
.globl irq7
irq7:
	pushl $7
	jmp irq_common_stub
.globl irq8
irq8:
	pushl $8
	jmp irq_common_stub
.globl irq9
irq9:
	pushl $9
	jmp irq_common_stub
.globl irq10
irq10:
	pushl $10
	jmp irq_common_stub
.globl irq11
irq11:
	pushl $11
	jmp irq_common_stub
.globl irq12
irq12:
	pushl $12
	jmp irq_common_stub
.globl irq13
irq13:
	pushl $13
	jmp irq_common_stub
.globl irq14
irq14:
	pushl $14
	jmp irq_common_stub
.globl irq15
irq15:
	pushl $15
	jmp irq_common_stub
