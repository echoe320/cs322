#pragma once
#include "L3.h"
#include "instructionSelect.h"
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
      void visit(Instruction_ret_not *i);
      void visit(Instruction_ret_t *i);
      void visit(Instruction_assignment *i);
      void visit(Instruction_load *i);
      void visit(Instruction_math *i);
      void visit(Instruction_store *i);
      void visit(Instruction_compare *i);
      void visit(Instruction_br_label *i);
      void visit(Instruction_br_t *i);
      void visit(Instruction_call_noassign *i);
      void visit(Instruction_call_assignment *i);
      void visit(Instruction_label *i);
  };
  AnalysisResult* computeLiveness(Function* function);
}