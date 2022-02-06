.text
	.globl go
go:
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	call _main
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	retq
_main:
	subq $48, %rsp
	movq %rbx, 40(%rsp)
	movq %rbp, 32(%rsp)
	movq %r15, 24(%rsp)
	movq %r14, 16(%rsp)
	movq %r13, 8(%rsp)
	movq %r12, 0(%rsp)
	movq $1, %r15
	movq $2, %rbp
	movq $3, %r14
	movq $4, %r13
	movq $5, %r12
	movq $6, %rbx
	movq %r15, %rdi
	imulq $2, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq %r15, %rdi
	movq %rbp, %rsi
	movq %r14, %rdx
	movq %r13, %rcx
	movq %r12, %r8
	movq %rbx, %r9
	movq $_call_label0, -8(%rsp)
	subq $8, %rsp
	jmp _firsttest_helper
	_call_label0:
	movq %rax, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq %r15, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq 0(%rsp), %r12
	movq 8(%rsp), %r13
	movq 16(%rsp), %r14
	movq 24(%rsp), %r15
	movq 32(%rsp), %rbp
	movq 40(%rsp), %rbx
	addq $48, %rsp
	retq
_firsttest_helper:
	movq %rdi, %rax
	movq %rsi, %rsi
	movq %rdx, %r10
	movq %rcx, %rdi
	movq %r8, %rdx
	movq %r9, %rcx
	addq $1, %rax
	addq %r10, %rsi
	addq %rdx, %rdi
	addq %rdi, %rsi
	addq %rcx, %rsi
	movq %rax, %rdi
	addq %rsi, %rdi
	movq %rdi, %rax
		retq
