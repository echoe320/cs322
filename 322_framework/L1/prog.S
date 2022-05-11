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
	_call_label0:
	jmp _call_label1
	_call_label1:
	movq $3, %rdi
	sarq $1, %rdi
	movq %rdi, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	addq $4, %rdi
	movq %rdi, %rdi
	movq $1, %rsi
	call allocate
	movq %rax, %rsi
	movq $3, 8(%rsi)
	movq $3, 16(%rsi)
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rdx
	addq $16, %rdx
	movq %rsi, %rdi
	addq %rdx, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	call print
		retq
