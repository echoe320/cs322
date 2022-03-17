#pragma once

#include <L2.h>

namespace L2{
  void create_liveness_list(Program p);

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
}