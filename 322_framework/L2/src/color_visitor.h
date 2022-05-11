#pragma once

#include <L2.h>
#include <graph_coloring.h>

namespace L2{
  Function* color2reg(Function* f, ColorGraph* cgraph);

  class Color_Visitors : public Visitor {
    public:
      Color_Visitors(ColorGraph* cgraph);
      ColorGraph* cgraph;
      std::vector<Instruction*> new_inst;
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