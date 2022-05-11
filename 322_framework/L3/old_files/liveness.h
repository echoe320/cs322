#pragma once
#include "L3.h"
#include "instruction_select.h"
#include <string>
#include <iostream>

namespace L3 {
  struct AnalysisResult {
    std::map<Instruction*, std::set<Item*>> gens; 
    std::map<Instruction*, std::set<Item*>> kills; 
    std::map<Instruction*, std::set<Item*>> ins; 
    std::map<Instruction*, std::set<Item*>> outs; 
  };

  class GenKill: public Visitor {
    public: 

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

      // void VisitInstruction(Instruction_ret_not *i);
      // void VisitInstruction(Instruction_ret_t *i);
      // void VisitInstruction(Instruction_assignment *i);
      // void VisitInstruction(Instruction_load *i);
      // void VisitInstruction(Instruction_math *i);
      // void VisitInstruction(Instruction_store *i);
      // void VisitInstruction(Instruction_compare *i);
      // void VisitInstruction(Instruction_br_label *i);
      // void VisitInstruction(Instruction_br_t *i);
      // void VisitInstruction(Instruction_call_noassign *i);
      // void VisitInstruction(Instruction_call_assignment *i);
      // void VisitInstruction(Instruction_label *i);
  };
  AnalysisResult* computeLiveness(Function* function);
}