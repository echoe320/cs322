#pragma once

#include <graph_coloring.h>
#include <L2.h>

namespace L2{
  void generate_L1_file(Program p);

  class Gen_Code_Visitors : public Visitor {
    public:
      Gen_Code_Visitors(std::ofstream &outF, Function* f, ColorGraph* color_graph);
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
      std::ofstream &outputFile;
      Function* f;
      ColorGraph* color_graph;
  };
}