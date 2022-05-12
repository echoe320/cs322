#pragma once

#include <L3.h>

namespace L3{
  void create_liveness_list(Function* f);

  class Gen_Kill_Visitors : public Visitor {
    void VisitInstruction (Instruction_ret_not *element) override;
    virtual void VisitInstruction(Instruction_ret_t *element) override;
    void VisitInstruction (Instruction_assignment *element) override;
    void VisitInstruction (Instruction_arithmetic *element) override;
    // void VisitInstruction (Instruction_crement *element) override;
    // void VisitInstruction (Instruction_shift *element) override;
    void VisitInstruction (Instruction_cmp *element) override;
    // void VisitInstruction (Instruction_cjump *element) override;
    // void VisitInstruction (Instruction_lea *element) override;
    void VisitInstruction (Instruction_calls *element) override;
    // void VisitInstruction (Instruction_runtime *element) override;
    void VisitInstruction (Instruction_label *element) override;
    // void VisitInstruction (Instruction_goto *element) override;
    // void VisitInstruction (Instruction_stackarg *element) override;
    void VisitInstruction(Instruction_load *element) override;
    void VisitInstruction(Instruction_store *element) override;
    void VisitInstruction(Instruction_br_label *element) override;
    void VisitInstruction(Instruction_br_t *element) override;
    void VisitInstruction(Instruction_call_noassign *element) override;
    void VisitInstruction(Instruction_call_assignment *element) override;
  };
}
