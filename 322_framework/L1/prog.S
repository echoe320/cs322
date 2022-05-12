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
	movq $_myOtherF, %r10
	movq $2, %rdi
	movq $_call_label1, -8(%rsp)
	subq $8, %rsp
	jmp *%r10
	_call_label1:
	movq %rax, %r10
	movq %r10, %rdi
	call print
	retq
_myOtherF:
	movq %rdi, %r10
	_call_label_ciao_2:
	movq %r10, %r10
	addq $1, %r10
	movq %r10, %rax
	retq
