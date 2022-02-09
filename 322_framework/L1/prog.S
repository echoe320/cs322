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
	subq $16, %rsp
	movq %r13, 8(%rsp)
	movq %r12, 0(%rsp)
	movq $1, %r12
	movq $2, %rdi
	movq %r12, %rsi
	addq %rdi, %rsi
	movq $3, %rdi
	imulq %rdi, %rsi
	movq $5, %rdi
	andq %rdi, %rsi
	movq %rsi, %rdi
	movq $_call_label1, -8(%rsp)
	subq $8, %rsp
	jmp _quadrupleInt
	_call_label1:
	movq %rax, %r13
	movq %r13, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq %r12, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rdi
	call print
	movq %r13, %rdi
	movq $_call_label2, -8(%rsp)
	subq $8, %rsp
	jmp _encode
	_call_label2:
	movq %rax, %rdi
	movq %rdi, %rdi
	call print
	movq $0, %r12
	_call_label0:
	addq $1, %r12
	cmpq $4, %r12
	setl %r13b
	movzbq %r13b, %r13
	movq %r13, %rdi
	movq $_call_label3, -8(%rsp)
	subq $8, %rsp
	jmp _encode
	_call_label3:
	movq %rax, %r13
	movq %r13, %rdi
	call print
	cmpq $1, %r13
	je _call_label0
	movq %r12, %rdi
	movq $_call_label4, -8(%rsp)
	subq $8, %rsp
	jmp _encode
	_call_label4:
	movq %rax, %r12
	movq %r12, %rdi
	call print
	movq 0(%rsp), %r12
	movq 8(%rsp), %r13
	addq $16, %rsp
	retq
_quadrupleInt:
	movq %rdi, %rdi
	imulq $4, %rdi
	movq %rdi, %rax
		retq
_encode:
	movq %rdi, %rdi
	salq $1, %rdi
	addq $1, %rdi
	movq %rdi, %rax
		retq
