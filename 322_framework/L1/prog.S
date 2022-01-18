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
	subq $64, %rsp
	_call_label0:
	movq $0, %rdi
	movq %rdi, 40(%rsp)
	movq $0, %rdi
	movq %rdi, 48(%rsp)
	movq $0, %rdi
	movq %rdi, 32(%rsp)
	movq $601, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rdi, %rsi
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rsi, %rsi
	imulq %rdi, %rsi
	movq %rsi, %rsi
	salq $1, %rsi
	movq %rsi, %rsi
	addq $1, %rsi
	movq %rsi, %rsi
	addq $6, %rsi
	movq %rsi, %rdi
	movq $1, %rsi
	call allocate
	movq %rax, %rdi
	movq %rdi, 32(%rsp)
	movq 32(%rsp), %rdi
	movq $5, 8(%rdi)
	movq 32(%rsp), %rdi
	movq 56(%rsp), %rsi
	movq %rsi, 16(%rdi)
	movq 32(%rsp), %rsi
	movq 56(%rsp), %rdi
	movq %rdi, 24(%rsi)
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rdi, %rsi
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rsi, %rsi
	imulq %rdi, %rsi
	movq %rsi, %rsi
	salq $1, %rsi
	movq %rsi, %rsi
	addq $1, %rsi
	movq %rsi, %rsi
	addq $6, %rsi
	movq %rsi, %rdi
	movq $1, %rsi
	call allocate
	movq %rax, %rdi
	movq %rdi, 48(%rsp)
	movq 48(%rsp), %rdi
	movq $5, 8(%rdi)
	movq 48(%rsp), %rsi
	movq 56(%rsp), %rdi
	movq %rdi, 16(%rsi)
	movq 48(%rsp), %rsi
	movq 56(%rsp), %rdi
	movq %rdi, 24(%rsi)
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rdi, %rsi
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rsi, %rsi
	imulq %rdi, %rsi
	movq %rsi, %rsi
	salq $1, %rsi
	movq %rsi, %rsi
	addq $1, %rsi
	movq %rsi, %rsi
	addq $6, %rsi
	movq %rsi, %rdi
	movq $1, %rsi
	call allocate
	movq %rax, %rdi
	movq %rdi, 40(%rsp)
	movq 40(%rsp), %rdi
	movq $5, 8(%rdi)
	movq 40(%rsp), %rsi
	movq 56(%rsp), %rdi
	movq %rdi, 16(%rsi)
	movq 40(%rsp), %rsi
	movq 56(%rsp), %rdi
	movq %rdi, 24(%rsi)
	movq $9, %rdi
	movq %rdi, 0(%rsp)
	movq $9, %rdi
	movq %rdi, 8(%rsp)
	movq $1, %rdi
	movq %rdi, 24(%rsp)
	jmp _call_label1
	_call_label1:
	movq 24(%rsp), %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	movq 0(%rsp), %rdi
	movq %rdi, %rdx
	movq %rdx, %rdx
	sarq $1, %rdx
	cmpq %rdx, %rsi
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	cmpq $1, %rsi
	je _call_label2
	jmp _call_label6
	_call_label2:
	movq $1, %rdi
	movq %rdi, 16(%rsp)
	jmp _call_label3
	_call_label3:
	movq 16(%rsp), %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	movq 8(%rsp), %rdi
	movq %rdi, %rdx
	movq %rdx, %rdx
	sarq $1, %rdx
	cmpq %rdx, %rsi
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	cmpq $1, %rsi
	je _call_label4
	jmp _call_label5
	_call_label4:
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	movq 48(%rsp), %rsi
	movq %rsi, %rsi
	movq 40(%rsp), %rdx
	movq %rdx, %rdx
	movq 24(%rsp), %rcx
	movq %rcx, %rcx
	movq 16(%rsp), %r8
	movq %r8, %r8
	movq $_call_label7, -8(%rsp)
	subq $8, %rsp
	jmp _computeAndPrint
	_call_label7:
	movq 16(%rsp), %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	movq $3, %rdx
	movq %rdx, %rdx
	sarq $1, %rdx
	movq %rsi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq %rdx, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 16(%rsp)
	jmp _call_label3
	_call_label5:
	movq 24(%rsp), %rdi
	movq %rdi, %rsi
	movq %rsi, %rsi
	sarq $1, %rsi
	movq $3, %rdx
	movq %rdx, %rdx
	sarq $1, %rdx
	movq %rsi, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	addq %rdx, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 24(%rsp)
	jmp _call_label1
	_call_label6:
	addq $64, %rsp
	retq
_computeAndPrint:
	subq $32, %rsp
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq %rsi, %rdi
	movq %rdi, 0(%rsp)
	movq %rdx, %rdi
	movq %rdi, 24(%rsp)
	movq %rcx, %rsi
	movq %r8, %rdi
	movq %rdi, 8(%rsp)
	_call_label_ciao_8:
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rsi, %rsi
	movq $_call_label_ciao_9, -8(%rsp)
	subq $8, %rsp
	jmp _initMatrix
	_call_label_ciao_9:
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq 8(%rsp), %rsi
	movq %rsi, %rsi
	movq $_call_label_ciao_10, -8(%rsp)
	subq $8, %rsp
	jmp _initMatrix
	_call_label_ciao_10:
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq 0(%rsp), %rsi
	movq %rsi, %rsi
	movq 24(%rsp), %rdx
	movq %rdx, %rdx
	movq $_call_label_ciao_11, -8(%rsp)
	subq $8, %rsp
	jmp _matrixMultiplication
	_call_label_ciao_11:
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq $_call_label_ciao_12, -8(%rsp)
	subq $8, %rsp
	jmp _totalSum
	_call_label_ciao_12:
	movq %rax, %rdi
	movq %rdi, %rdi
	call print
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq $_call_label_ciao_13, -8(%rsp)
	subq $8, %rsp
	jmp _totalSum
	_call_label_ciao_13:
	movq %rax, %rdi
	movq %rdi, %rdi
	call print
	movq 24(%rsp), %rdi
	movq %rdi, %rdi
	movq $_call_label_ciao_14, -8(%rsp)
	subq $8, %rsp
	jmp _totalSum
	_call_label_ciao_14:
	movq %rax, %rdi
	movq %rdi, %rdi
	call print
	addq $32, %rsp
	retq
_initMatrix:
	subq $32, %rsp
	movq %rdi, %rdx
	movq %rsi, %rsi
	_call_label_ciao__ciao_15:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rcx
	addq $16, %rcx
	movq %rdx, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %rcx
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rdi
	addq $16, %rdi
	movq %rdx, %r8
	addq %rdi, %r8
	movq 0(%r8), %rdi
	movq %rdi, 8(%rsp)
	movq $1, %r8
	jmp _call_label_ciao__ciao_16
	_call_label_ciao__ciao_16:
	movq %r8, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq %rcx, %r9
	movq %r9, %r9
	sarq $1, %r9
	cmpq %r9, %r10
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %r10
	movq %r10, %r10
	sarq $1, %r10
	cmpq $1, %r10
	je _call_label_ciao__ciao_17
	jmp _call_label_ciao__ciao_26
	_call_label_ciao__ciao_17:
	movq $1, %rdi
	movq %rdi, 0(%rsp)
	jmp _call_label_ciao__ciao_18
	_call_label_ciao__ciao_18:
	movq 0(%rsp), %rdi
	movq %rdi, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq 8(%rsp), %rdi
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	cmpq %r9, %r10
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %r10
	movq %r10, %r10
	sarq $1, %r10
	cmpq $1, %r10
	je _call_label_ciao__ciao_19
	jmp _call_label_ciao__ciao_25
	_call_label_ciao__ciao_19:
	movq %rsi, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq %r8, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %r10, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq %r9, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq 0(%rsp), %rdi
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %r10, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq %r9, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq 16(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 16(%rsp)
	movq %r8, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 24(%rsp)
	movq 24(%rsp), %rdi
	sarq $1, %rdi
	movq %rdi, 24(%rsp)
	movq $0, %r11
	cmpq $0, %rdx
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao_23
	jmp _call_label_ciao__ciao_20
	_call_label_ciao__ciao_20:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rdi
	addq $16, %rdi
	movq %rdx, %r10
	addq %rdi, %r10
	movq 0(%r10), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r9, %r11
	movq %r11, %r11
	salq $1, %r11
	movq %r11, %r11
	addq $1, %r11
	cmpq %rdi, %r9
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao_21
	jmp _call_label_ciao__ciao_23
	_call_label_ciao__ciao_21:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq %rdx, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %r10
	movq %r10, %r10
	sarq $1, %r10
	movq 24(%rsp), %rdi
	movq %rdi, %r11
	movq %r11, %r11
	salq $1, %r11
	movq %r11, %r11
	addq $1, %r11
	movq 24(%rsp), %rdi
	cmpq %r10, %rdi
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao_22
	jmp _call_label_ciao__ciao_23
	_call_label_ciao__ciao_22:
	jmp _call_label_ciao__ciao_24
	_call_label_ciao__ciao_23:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao_24
	_call_label_ciao__ciao_24:
	movq $0, %r10
	movq 24(%rdx), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r9, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	movq 24(%rsp), %rdi
	addq %rdi, %r10
	movq %r10, %rdi
	imulq $8, %rdi
	movq %rdi, %rdi
	addq $32, %rdi
	movq %rdx, %r9
	addq %rdi, %r9
	movq 16(%rsp), %rdi
	movq %rdi, 0(%r9)
	movq 0(%rsp), %rdi
	movq %rdi, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq $3, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %r10, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	addq %r9, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 0(%rsp)
	jmp _call_label_ciao__ciao_18
	_call_label_ciao__ciao_25:
	movq %r8, %r10
	movq %r10, %r10
	sarq $1, %r10
	movq $3, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %r10, %r8
	addq %r9, %r8
	movq %r8, %r8
	salq $1, %r8
	movq %r8, %r8
	addq $1, %r8
	jmp _call_label_ciao__ciao_16
	_call_label_ciao__ciao_26:
	addq $32, %rsp
	retq
_matrixMultiplication:
	subq $64, %rsp
	movq %rdi, %rdi
	movq %rdi, 16(%rsp)
	movq %rsi, %rdi
	movq %rdi, 40(%rsp)
	movq %rdx, %rdi
	movq %rdi, 32(%rsp)
	_call_label_ciao__ciao__ciao_27:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rsi
	addq $16, %rsi
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	addq %rsi, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, 48(%rsp)
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rsi
	addq $16, %rsi
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	addq %rsi, %rdi
	movq 0(%rdi), %rsi
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rdx
	addq $16, %rdx
	movq 40(%rsp), %rdi
	movq %rdi, %rdi
	addq %rdx, %rdi
	movq 0(%rdi), %rdx
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rcx
	addq $16, %rcx
	movq 40(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, 24(%rsp)
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rcx
	addq $16, %rcx
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %r9
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rcx
	addq $16, %rcx
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %r10
	movq %rsi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq %rdx, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	sete %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_28
	jmp _call_label_ciao__ciao__ciao_73
	_call_label_ciao__ciao__ciao_28:
	movq %r9, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 48(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	sete %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_29
	jmp _call_label_ciao__ciao__ciao_73
	_call_label_ciao__ciao__ciao_29:
	movq %r10, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 24(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	sete %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_30
	jmp _call_label_ciao__ciao__ciao_73
	_call_label_ciao__ciao__ciao_30:
	movq $1, %rdi
	movq %rdi, 56(%rsp)
	jmp _call_label_ciao__ciao__ciao_31
	_call_label_ciao__ciao__ciao_31:
	movq 56(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 48(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_32
	jmp _call_label_ciao__ciao__ciao_44
	_call_label_ciao__ciao__ciao_32:
	movq $1, %rdx
	jmp _call_label_ciao__ciao__ciao_33
	_call_label_ciao__ciao__ciao_33:
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 24(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_34
	jmp _call_label_ciao__ciao__ciao_43
	_call_label_ciao__ciao__ciao_34:
	movq $1, %r9
	jmp _call_label_ciao__ciao__ciao_35
	_call_label_ciao__ciao__ciao_35:
	movq %r9, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq %rsi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_36
	jmp _call_label_ciao__ciao__ciao_42
	_call_label_ciao__ciao__ciao_36:
	movq 56(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $0, %r10
	movq 32(%rsp), %rdi
	cmpq $0, %rdi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_40
	jmp _call_label_ciao__ciao__ciao_37
	_call_label_ciao__ciao__ciao_37:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r8, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r8
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_38
	jmp _call_label_ciao__ciao__ciao_40
	_call_label_ciao__ciao__ciao_38:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rcx, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %rcx
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_39
	jmp _call_label_ciao__ciao__ciao_40
	_call_label_ciao__ciao__ciao_39:
	jmp _call_label_ciao__ciao__ciao_41
	_call_label_ciao__ciao__ciao_40:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao_41
	_call_label_ciao__ciao__ciao_41:
	movq $0, %r10
	movq 32(%rsp), %rdi
	movq 24(%rdi), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r8, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	addq %rcx, %r10
	movq %r10, %rcx
	imulq $8, %rcx
	movq %rcx, %rcx
	addq $32, %rcx
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq $1, 0(%rdi)
	movq %r9, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %r9
	addq %r8, %r9
	movq %r9, %r9
	salq $1, %r9
	movq %r9, %r9
	addq $1, %r9
	jmp _call_label_ciao__ciao__ciao_35
	_call_label_ciao__ciao__ciao_42:
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rdx
	addq %r8, %rdx
	movq %rdx, %rdx
	salq $1, %rdx
	movq %rdx, %rdx
	addq $1, %rdx
	jmp _call_label_ciao__ciao__ciao_33
	_call_label_ciao__ciao__ciao_43:
	movq 56(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	addq %r8, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 56(%rsp)
	movq 56(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 56(%rsp)
	jmp _call_label_ciao__ciao__ciao_31
	_call_label_ciao__ciao__ciao_44:
	movq $1, %rdi
	movq %rdi, 8(%rsp)
	jmp _call_label_ciao__ciao__ciao_45
	_call_label_ciao__ciao__ciao_45:
	movq 8(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 48(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_46
	jmp _call_label_ciao__ciao__ciao_73
	_call_label_ciao__ciao__ciao_46:
	movq $1, %rdx
	jmp _call_label_ciao__ciao__ciao_47
	_call_label_ciao__ciao__ciao_47:
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 24(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_48
	jmp _call_label_ciao__ciao__ciao_72
	_call_label_ciao__ciao__ciao_48:
	movq $1, %rdi
	movq %rdi, 0(%rsp)
	jmp _call_label_ciao__ciao__ciao_49
	_call_label_ciao__ciao__ciao_49:
	movq 0(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq %rsi, %r8
	movq %r8, %r8
	sarq $1, %r8
	cmpq %r8, %rcx
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	cmpq $1, %rcx
	je _call_label_ciao__ciao__ciao_50
	jmp _call_label_ciao__ciao__ciao_71
	_call_label_ciao__ciao__ciao_50:
	movq 8(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq 0(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq $0, %r9
	movq 16(%rsp), %rdi
	cmpq $0, %rdi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_54
	jmp _call_label_ciao__ciao__ciao_51
	_call_label_ciao__ciao__ciao_51:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r9
	addq $16, %r9
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	addq %r9, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rcx, %r9
	movq %r9, %r9
	salq $1, %r9
	movq %r9, %r9
	addq $1, %r9
	cmpq %rdi, %rcx
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_52
	jmp _call_label_ciao__ciao__ciao_54
	_call_label_ciao__ciao__ciao_52:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r9
	addq $16, %r9
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	addq %r9, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r8, %r9
	movq %r9, %r9
	salq $1, %r9
	movq %r9, %r9
	addq $1, %r9
	cmpq %rdi, %r8
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_53
	jmp _call_label_ciao__ciao__ciao_54
	_call_label_ciao__ciao__ciao_53:
	jmp _call_label_ciao__ciao__ciao_55
	_call_label_ciao__ciao__ciao_54:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao_55
	_call_label_ciao__ciao__ciao_55:
	movq $0, %r9
	movq 16(%rsp), %rdi
	movq 24(%rdi), %rdi
	movq $1, %r10
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r10, %r10
	imulq %rdi, %r10
	movq %rcx, %rdi
	imulq %r10, %rdi
	movq %r9, %r9
	addq %rdi, %r9
	movq %r9, %r9
	addq %r8, %r9
	movq %r9, %rcx
	imulq $8, %rcx
	movq %rcx, %rcx
	addq $32, %rcx
	movq 16(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %rcx
	movq 0(%rsp), %rdi
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %rdx, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq $0, %r10
	movq 40(%rsp), %rdi
	cmpq $0, %rdi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_59
	jmp _call_label_ciao__ciao__ciao_56
	_call_label_ciao__ciao__ciao_56:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 40(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r9, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r9
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_57
	jmp _call_label_ciao__ciao__ciao_59
	_call_label_ciao__ciao__ciao_57:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 40(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r8, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r8
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_58
	jmp _call_label_ciao__ciao__ciao_59
	_call_label_ciao__ciao__ciao_58:
	jmp _call_label_ciao__ciao__ciao_60
	_call_label_ciao__ciao__ciao_59:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao_60
	_call_label_ciao__ciao__ciao_60:
	movq $0, %r10
	movq 40(%rsp), %rdi
	movq 24(%rdi), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r9, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	addq %r8, %r10
	movq %r10, %r8
	imulq $8, %r8
	movq %r8, %r8
	addq $32, %r8
	movq 40(%rsp), %rdi
	movq %rdi, %rdi
	addq %r8, %rdi
	movq 0(%rdi), %rdi
	movq %rcx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %r9
	imulq %r8, %r9
	movq %r9, %r9
	salq $1, %r9
	movq %r9, %r9
	addq $1, %r9
	movq 8(%rsp), %rdi
	movq %rdi, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $0, %r10
	movq 32(%rsp), %rdi
	cmpq $0, %rdi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_64
	jmp _call_label_ciao__ciao__ciao_61
	_call_label_ciao__ciao__ciao_61:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r8, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r8
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_62
	jmp _call_label_ciao__ciao__ciao_64
	_call_label_ciao__ciao__ciao_62:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rcx, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %rcx
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_63
	jmp _call_label_ciao__ciao__ciao_64
	_call_label_ciao__ciao__ciao_63:
	jmp _call_label_ciao__ciao__ciao_65
	_call_label_ciao__ciao__ciao_64:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao_65
	_call_label_ciao__ciao__ciao_65:
	movq $0, %r10
	movq 32(%rsp), %rdi
	movq 24(%rdi), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r8, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	addq %rcx, %r10
	movq %r10, %rcx
	imulq $8, %rcx
	movq %rcx, %rcx
	addq $32, %rcx
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %rcx, %rdi
	movq 0(%rdi), %rcx
	movq %rcx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $9, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rcx
	imulq %r8, %rcx
	movq %rcx, %rcx
	salq $1, %rcx
	movq %rcx, %rcx
	addq $1, %rcx
	movq %rcx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq %r9, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rcx
	addq %r8, %rcx
	movq %rcx, %rcx
	salq $1, %rcx
	movq %rcx, %rcx
	addq $1, %rcx
	movq 8(%rsp), %rdi
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %rdx, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq $0, %r10
	movq 32(%rsp), %rdi
	cmpq $0, %rdi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_69
	jmp _call_label_ciao__ciao__ciao_66
	_call_label_ciao__ciao__ciao_66:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r9, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r9
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_67
	jmp _call_label_ciao__ciao__ciao_69
	_call_label_ciao__ciao__ciao_67:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r8, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r8
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao_68
	jmp _call_label_ciao__ciao__ciao_69
	_call_label_ciao__ciao__ciao_68:
	jmp _call_label_ciao__ciao__ciao_70
	_call_label_ciao__ciao__ciao_69:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao_70
	_call_label_ciao__ciao__ciao_70:
	movq $0, %r10
	movq 32(%rsp), %rdi
	movq 24(%rdi), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r9, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	addq %r8, %r10
	movq %r10, %r8
	imulq $8, %r8
	movq %r8, %r8
	addq $32, %r8
	movq 32(%rsp), %rdi
	movq %rdi, %rdi
	addq %r8, %rdi
	movq %rcx, 0(%rdi)
	movq 0(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	addq %r8, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 0(%rsp)
	movq 0(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 0(%rsp)
	jmp _call_label_ciao__ciao__ciao_49
	_call_label_ciao__ciao__ciao_71:
	movq %rdx, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rdx
	addq %r8, %rdx
	movq %rdx, %rdx
	salq $1, %rdx
	movq %rdx, %rdx
	addq $1, %rdx
	jmp _call_label_ciao__ciao__ciao_47
	_call_label_ciao__ciao__ciao_72:
	movq 8(%rsp), %rdi
	movq %rdi, %rcx
	movq %rcx, %rcx
	sarq $1, %rcx
	movq $3, %r8
	movq %r8, %r8
	sarq $1, %r8
	movq %rcx, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	addq %r8, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 8(%rsp)
	jmp _call_label_ciao__ciao__ciao_45
	_call_label_ciao__ciao__ciao_73:
	addq $64, %rsp
	retq
_totalSum:
	subq $16, %rsp
	movq %rdi, %rsi
	_call_label_ciao__ciao__ciao__ciao_74:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %rdx
	addq $16, %rdx
	movq %rsi, %rdi
	addq %rdx, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, 0(%rsp)
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rdi
	addq $16, %rdi
	movq %rsi, %rdx
	addq %rdi, %rdx
	movq 0(%rdx), %r8
	movq $1, %rdx
	movq $1, %rcx
	jmp _call_label_ciao__ciao__ciao__ciao_75
	_call_label_ciao__ciao__ciao__ciao_75:
	movq %rdx, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq 0(%rsp), %rdi
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	cmpq %r9, %rax
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rax
	movq %rax, %rax
	sarq $1, %rax
	cmpq $1, %rax
	je _call_label_ciao__ciao__ciao__ciao_76
	jmp _call_label_ciao__ciao__ciao__ciao_85
	_call_label_ciao__ciao__ciao__ciao_76:
	movq $1, %rdi
	movq %rdi, 8(%rsp)
	jmp _call_label_ciao__ciao__ciao__ciao_77
	_call_label_ciao__ciao__ciao__ciao_77:
	movq 8(%rsp), %rdi
	movq %rdi, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq %r8, %r9
	movq %r9, %r9
	sarq $1, %r9
	cmpq %r9, %rax
	setl %dil
	movzbq %dil, %rdi
	movq %rdi, %rdi
	salq $1, %rdi
	movq %rdi, %rdi
	addq $1, %rdi
	movq %rdi, %rax
	movq %rax, %rax
	sarq $1, %rax
	cmpq $1, %rax
	je _call_label_ciao__ciao__ciao__ciao_78
	jmp _call_label_ciao__ciao__ciao__ciao_84
	_call_label_ciao__ciao__ciao__ciao_78:
	movq %rdx, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq 8(%rsp), %rdi
	movq %rdi, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq $0, %r10
	cmpq $0, %rsi
	sete %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao__ciao_82
	jmp _call_label_ciao__ciao__ciao__ciao_79
	_call_label_ciao__ciao__ciao__ciao_79:
	movq $0, %rdi
	imulq $8, %rdi
	movq %rdi, %r10
	addq $16, %r10
	movq %rsi, %rdi
	addq %r10, %rdi
	movq 0(%rdi), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r9, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %r9
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao__ciao_80
	jmp _call_label_ciao__ciao__ciao__ciao_82
	_call_label_ciao__ciao__ciao__ciao_80:
	movq $1, %rdi
	imulq $8, %rdi
	movq %rdi, %rdi
	addq $16, %rdi
	movq %rsi, %r10
	addq %rdi, %r10
	movq 0(%r10), %rdi
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %rax, %r10
	movq %r10, %r10
	salq $1, %r10
	movq %r10, %r10
	addq $1, %r10
	cmpq %rdi, %rax
	setl %dil
	movzbq %dil, %rdi
	cmpq $1, %rdi
	je _call_label_ciao__ciao__ciao__ciao_81
	jmp _call_label_ciao__ciao__ciao__ciao_82
	_call_label_ciao__ciao__ciao__ciao_81:
	jmp _call_label_ciao__ciao__ciao__ciao_83
	_call_label_ciao__ciao__ciao__ciao_82:
	movq $0, %rdi
	call array_tensor_error_null
	jmp _call_label_ciao__ciao__ciao__ciao_83
	_call_label_ciao__ciao__ciao__ciao_83:
	movq $0, %r10
	movq 24(%rsi), %rdi
	movq $1, %r11
	movq %rdi, %rdi
	sarq $1, %rdi
	movq %r11, %r11
	imulq %rdi, %r11
	movq %r9, %rdi
	imulq %r11, %rdi
	movq %r10, %r10
	addq %rdi, %r10
	movq %r10, %r10
	addq %rax, %r10
	movq %r10, %r9
	imulq $8, %r9
	movq %r9, %r9
	addq $32, %r9
	movq %rsi, %rdi
	addq %r9, %rdi
	movq 0(%rdi), %rdi
	movq %rcx, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq %rdi, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %rax, %rcx
	addq %r9, %rcx
	movq %rcx, %rcx
	salq $1, %rcx
	movq %rcx, %rcx
	addq $1, %rcx
	movq 8(%rsp), %rdi
	movq %rdi, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq $3, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %rax, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	addq %r9, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	salq $1, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	movq %rdi, %rdi
	movq %rdi, 8(%rsp)
	movq 8(%rsp), %rdi
	addq $1, %rdi
	movq %rdi, 8(%rsp)
	jmp _call_label_ciao__ciao__ciao__ciao_77
	_call_label_ciao__ciao__ciao__ciao_84:
	movq %rdx, %rax
	movq %rax, %rax
	sarq $1, %rax
	movq $3, %r9
	movq %r9, %r9
	sarq $1, %r9
	movq %rax, %rdx
	addq %r9, %rdx
	movq %rdx, %rdx
	salq $1, %rdx
	movq %rdx, %rdx
	addq $1, %rdx
	jmp _call_label_ciao__ciao__ciao__ciao_75
	_call_label_ciao__ciao__ciao__ciao_85:
	movq %rcx, %rax
	addq $16, %rsp
	retq
