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
	jmp _begin_work
	_ret:
	retq
	_begin_work:
	movq $13, %rdi
	movq $13, %rsi
	movq $13, %rcx
	movq $13, %rdx
	movq $13, %r8
	movq $13, %r9
	movq $_ret, -8(%rsp)
	movq $13, -16(%rsp)
	movq $13, -24(%rsp)
	subq $24, %rsp
	jmp _printValues
	jmp _ret
_printValues:
	movq 8(%rsp), %r10
	addq $16, %rsp
retq
