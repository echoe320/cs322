.text
	.globl go
go:
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	call _go
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	retq
_go:
	subq $0, %rsp
	movq $5, %rdi
	movq %rdi, %rax
	movq _testFunction, %rsi
	movq %rdi, 0(%rsp)
	movq 8(%rsp), %rdi
	movq $_a_label, -8(%rsp)
	addq %rsi, %rdi
	subq %rsi, %rdi
	imulq %rsi, %rdi
	andq %rsi, %rdi
	inc %rdi
	dec %rdi
	salq %cl, %rdi
	sarq $3, %rdi
	cmpq %rbx, %rax
	setle %dil
	movzbq %dil, %rdi
	cmpq $10, %rax
	setle %dil
	movzbq %dil, %rdi
	cmpq $10, %rax
	setge %dil
	movzbq %dil, %rdi
	movq $1, %rdi
	movq $0, %rax
	_a_label:
	jmp _a_label
	cmpq %rdi, %rax
	jle _a_label
	jmp _a_label
	
	lea (%rdi, %rsi, 4), %rax
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	addq $0, %rsp
	retq
	movq $_rett, -8(%rsp)
	addq $0, %rsp
	retq
	_rett:
	addq $0, %rsp
	retq
_testFunction:
	subq $16, %rsp
	addq $16, %rsp
	retq
