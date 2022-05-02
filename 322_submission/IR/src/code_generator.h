#pragma once
#include <iostream>
#include <fstream>

#include <IR.h>

namespace IR{

  void generate_code(Program p);

  class IR_Visitors : public Visitor {
    public:
      IR_Visitors(Function *f, std::ofstream &s);
      void VisitInstruction(Instruction_def *element) override;
      void VisitInstruction(Instruction_assignment *element) override;
      void VisitInstruction(Instruction_op *element) override;
      void VisitInstruction(Instruction_load *element) override;
      void VisitInstruction(Instruction_store *element) override;
      void VisitInstruction(Instruction_length *element) override;
      void VisitInstruction(Instruction_call *element) override;
      void VisitInstruction(Instruction_call_assign *element) override;
      void VisitInstruction(Instruction_array *element) override;
      void VisitInstruction(Instruction_tuple *element) override;
      void VisitInstruction(Instruction_label *element) override;
      void VisitInstruction(te_br_label *element) override;
      void VisitInstruction(te_br_t *element) override;
      void VisitInstruction(te_return *element) override;
      void VisitInstruction(te_return_t *element) override;
    private: 
      std::ofstream &outputFile;
      Function* f;
  };

}