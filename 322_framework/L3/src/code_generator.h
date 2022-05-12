#pragma once

#include <L3.h>

namespace L3{
  void generate_L2_file(Program p);
  
  /* Visitor Pattern */
  class Gen_Code_Visitors : public Visitor {
    public:
      Gen_Code_Visitors(std::ofstream &outF); 
      // vector<std::string> L2_instructions;

      virtual void VisitInstruction(Instruction_ret_not *element) override;
      virtual void VisitInstruction(Instruction_ret_t *element) override;
      virtual void VisitInstruction(Instruction_assignment *element) override;
      virtual void VisitInstruction(Instruction_load *element) override;
      virtual void VisitInstruction(Instruction_arithmetic *element) override;
      virtual void VisitInstruction(Instruction_store *element) override;
      virtual void VisitInstruction(Instruction_cmp *element) override;
      virtual void VisitInstruction(Instruction_br_label *element) override;
      virtual void VisitInstruction(Instruction_br_t *element) override;
      virtual void VisitInstruction(Instruction_call_noassign *element) override;
      virtual void VisitInstruction(Instruction_call_assignment *element) override;
      virtual void VisitInstruction(Instruction_label *element) override;
      std::ofstream &outputFile;
  };
}
