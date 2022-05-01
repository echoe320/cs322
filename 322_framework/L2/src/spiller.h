#pragma once

#include <L2.h>

namespace L2{
  void spill_one_var(Function* f, std::string toSpill, std::string prefix);

  class Spill_Visitors : public Visitor {
    public:
      Spill_Visitors(std::string toSpill, std::string prefix);
      int getCount();
      int numSpilled = 0;
      std::vector<Instruction*> getInstructions();
      // std::vector<std::pair<bool, bool>> get_spill_use();
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

    private:
      std::string var;
      std::string prefix;
      int count = 0;
      std::vector<std::pair<bool, bool>> RWflags;
      std::vector<Instruction*> new_inst;
      Instruction* loadvar();
      Instruction* storevar();
  };
}