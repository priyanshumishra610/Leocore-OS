.intel_syntax noprefix
.global cpu_get_vendor

# void cpu_get_vendor(char out[13])
# Writes EBX, EDX, ECX from CPUID(0) into out and null-terminates
cpu_get_vendor:
	push ebp
	mov ebp, esp
	push ebx
	xor eax, eax
	cpuid
	mov esi, [ebp+8]   # out
	mov [esi+0], ebx
	mov [esi+4], edx
	mov [esi+8], ecx
	mov byte ptr [esi+12], 0
	pop ebx
	pop ebp
	ret
