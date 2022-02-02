#pragma once

#include <L2.h>

namespace L2{
  int caller_reg_list[] = {reg::r10, reg::r11, reg::r8, reg::r9, reg::rax, reg::rcx, reg::rdi, reg::rdx, reg::rsi};
  int callee_reg_list[] = {reg::r12, reg::r13, reg::r14, reg::r15, reg::rbp, reg::rbx};
  int arg_reg_list[] = {reg::rdi, reg::rsi, reg::rdx, reg::rcx, reg::r8, reg::r9};

  class Gen_Kill_Visitors : public Visitor {
    void VisitInstruction (Instruction_ret *element) override;
    void VisitInstruction (Instruction_assignment *element) override;
    void VisitInstruction (Instruction_arithmetic *element) override;
    void VisitInstruction (Instruction_crement *element) override;
    void VisitInstruction (Instruction_shift *element) override;
    void VisitInstruction (Instruction_cmp *element) override;
    void VisitInstruction (Instruction_cjump *element) override;
    void VisitInstruction (Instruction_lea *element) override;
    void VisitInstruction (Instruction_calls *element) override;
    void VisitInstruction (Instruction_runtime *element) override;
    void VisitInstruction (Instruction_label *element) override;
    void VisitInstruction (Instruction_goto *element) override;
    void VisitInstruction (Instruction_stackarg *element) override;
  };

  class In_Out_Visitors : public Visitor {
    void VisitInstruction (Instruction_ret *element) override;
    void VisitInstruction (Instruction_assignment *element) override;
    void VisitInstruction (Instruction_arithmetic *element) override;
    void VisitInstruction (Instruction_crement *element) override;
    void VisitInstruction (Instruction_shift *element) override;
    void VisitInstruction (Instruction_cmp *element) override;
    void VisitInstruction (Instruction_cjump *element) override;
    void VisitInstruction (Instruction_lea *element) override;
    void VisitInstruction (Instruction_calls *element) override;
    void VisitInstruction (Instruction_runtime *element) override;
    void VisitInstruction (Instruction_label *element) override;
    void VisitInstruction (Instruction_goto *element) override;
    void VisitInstruction (Instruction_stackarg *element) override;
  };
  void create_liveness_list(Program p);
}