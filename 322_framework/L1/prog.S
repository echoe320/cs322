   .text
   .globl go

go:
   # save callee-saved registers
   pushq %rbx
   pushq %rbp
   pushq %r12
   pushq %r13
   pushq %r14
   pushq %r15

   call _firsttest

   # restore callee-saved registers and return
   popq %r15
   popq %r14
   popq %r13
   popq %r12
   popq %rbp
   popq %rbx
   retq


_firsttest:
   subq $16, %rsp # Allocate spills
   movq $3, %r10
   movq $5, %r11
   movq %r10, 0(%rsp)
   movq %r11, 8(%rsp)
   movq $_ft_helper_ret, -8(%rsp)
   subq $8, %rsp      # Allocate extra arguments on the stack as well as the return address 
   jmp _firsttest_helper        # Call to :firsttest_helper
_ft_helper_ret:
   movq 0(%rsp), %r10
   movq 8(%rsp), %r11
   movq %rax, %rdi
   call print # runtime system call
   movq %r10, %rdi
   call print # runtime system call
   addq $16, %rsp # Free stack locations
   retq


_firsttest_helper:
   imulq $5, %r10
   movq %r10, %rax
   retq

