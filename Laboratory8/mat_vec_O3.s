	.file	"mat_vec.c"
	.text
	.p2align 4
	.globl	mat_vec
	.type	mat_vec, @function
mat_vec:
.LFB39:
	.cfi_startproc
	endbr64
	movq	%rsi, %r11
	testl	%ecx, %ecx
	jle	.L1
	movslq	%ecx, %r8
	xorl	%r10d, %r10d
	salq	$3, %r8
	leaq	(%rsi,%r8), %r9
	.p2align 4,,10
	.p2align 3
.L4:
	movq	$0x000000000, (%rdx)
	movq	%r11, %rax
	movq	%rdi, %rsi
	pxor	%xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L3:
	movsd	(%rsi), %xmm0
	mulsd	(%rax), %xmm0
	addq	$8, %rax
	addq	%r8, %rsi
	addsd	%xmm0, %xmm1
	movsd	%xmm1, (%rdx)
	cmpq	%rax, %r9
	jne	.L3
	addl	$1, %r10d
	addq	$8, %rdx
	addq	$8, %rdi
	cmpl	%r10d, %ecx
	jne	.L4
.L1:
	ret
	.cfi_endproc
.LFE39:
	.size	mat_vec, .-mat_vec
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
