#pragma once

#include <L2.h>

namespace L2{
  // class reg_var {
  //   //enum reg {regs, var}; //num -> string
  //   int enum_reg;
  //   string v; // reg string also?
  // };
  class Gen_Kill_Visitors : public Visitor {
    void VisitInstruction (const Instruction_ret *element) override;
    void VisitInstruction (const Instruction_assignment *element) override;
    void VisitInstruction (const Instruction_arithmetic *element) override;
    void VisitInstruction (const Instruction_crement *element) override;
    void VisitInstruction (const Instruction_shift *element) override;
    void VisitInstruction (const Instruction_cmp *element) override;
    void VisitInstruction (const Instruction_cjump *element) override;
    void VisitInstruction (const Instruction_lea *element) override;
    void VisitInstruction (const Instruction_calls *element) override;
    void VisitInstruction (const Instruction_runtime *element) override;
    void VisitInstruction (const Instruction_label *element) override;
    void VisitInstruction (const Instruction_goto *element) override;
    void VisitInstruction (const Instruction_stackarg *element) override;
  };
  void create_liveness_list(Program p);
}