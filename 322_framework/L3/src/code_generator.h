#pragma once
#include <iostream>
#include <fstream>

#include <L3.h>

namespace L3{

  void generate_code(Program p);

  class L3_Visitors : public Visitor {
    public:
      L3_Visitors(Function *f, std::ofstream &s);
      void VisitInstruction(Instruction_assignment *element) override;
      void VisitInstruction(Instruction_op *element) override;
      void VisitInstruction(Instruction_load *element) override;
      void VisitInstruction(Instruction_store *element) override;
      void VisitInstruction(Instruction_call *element) override;
      void VisitInstruction(Instruction_call_assign *element) override;
      void VisitInstruction(Instruction_label *element) override;
      void VisitInstruction(Instruction_br_label *element) override;
      void VisitInstruction(Instruction_br_t *element) override;
      void VisitInstruction(Instruction_return *element) override;
      void VisitInstruction(Instruction_return_t *element) override;
    private: 
      std::ofstream &outputFile;
      Function* f;
  };

}