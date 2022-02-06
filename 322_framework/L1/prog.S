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
	movq $2, %rbx
	movq $3, %r12
	movq $4, %r13
	movq $5, %r14
	movq $6, %rbp
	movq %r15, %rdi
	imulq $2, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq %r15, %rdi
	movq %rbx, %rsi
	movq %r12, %rdx
	movq %r13, %rcx
	movq %r14, %r8
	movq %rbp, %r9
	movq $_call_label1, -8(%rsp)
	subq $8, %rsp
	jmp _firsttest_helper
	_call_label1:
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
	movq $6, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq $1, %rsi
	movq %rdi, %rdi
	movq %rsi, %rsi
	call allocate
	movq %rax, %rax
	movq %rax, %rcx
	addq $8, %rcx
	movq %rax, %r8
	addq $16, %r8
	movq %rax, %r9
	addq $24, %r9
	movq %rax, %rdi
	addq $32, %rdi
	movq %rax, %rdx
	addq $40, %rdx
	movq %rax, %rsi
	addq $48, %rsi
	movq $1, 0(%rcx)
	movq $2, 0(%r8)
	movq $3, 0(%r9)
	movq $4, 0(%rdi)
	movq $5, 0(%rdx)
	movq $6, 0(%rsi)
	movq $8, %rsi
	movq $1, %rdx
	_call_label0:
	movq %rax, %rdi
	addq %rsi, %rdi
	movq 0(%rdi), %rdi
	addq %rdi, %rdx
	addq $8, %rsi
	cmpq $48, %rsi
	setle %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label0
	salq $1, %rdx
	addq $1, %rdx
	movq %rdx, %rdi
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
