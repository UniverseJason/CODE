	.file	"code.c"
	.intel_syntax noprefix
	.text
	.globl	foo
	.type	foo, @function
foo:
	push	ebp			// save ebp
	mov	ebp, esp		// get new ebp
	sub	esp, 64			// alloc space for local veriables
	mov	DWORD PTR [ebp-4], 0
	jmp	.L2
.L3:
	mov	eax, DWORD PTR [ebp-4]
	mov	DWORD PTR [ebp-56+eax*4], 5
	add	DWORD PTR [ebp-4], 1
.L2:
	cmp	DWORD PTR [ebp-4], 12
	jle	.L3
	mov	eax, DWORD PTR [ebp-4]
	leave
	ret
	.size	foo, .-foo
	.globl	main
	.type	main, @function
main:
	push	ebp
	mov	ebp, esp
	call	foo
	mov	eax, 0
	pop	ebp
	ret
	.size	main, .-main
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
