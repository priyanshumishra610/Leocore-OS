.intel_syntax noprefix
.global sti_asm
.global cli_asm
.global hlt_asm

sti_asm:
	sti
	ret

cli_asm:
	cli
	ret

hlt_asm:
	hlt
	ret
