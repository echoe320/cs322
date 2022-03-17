#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <spiller.h>
#include <L2_parser.h>

// included libraries
#include <unordered_set>
#include <tuple>


namespace L2 {
    Spill_Visitors::Spill_Visitors(std::string toSpill) {
        this->var = toSpill;
    }
    
    void Spill_Visitors::VisitInstruction(Instruction_ret *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_assignment *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_arithmetic *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_crement *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_shift *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_cmp *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_cjump *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_lea *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_calls *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_runtime *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_label *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_goto *element){

    }

    void Spill_Visitors::VisitInstruction(Instruction_stackarg *element){

    }

  void spill_one_var(Function* f, std::string toSpill, std::string prefix) {
      auto spill_visitor = new Spill_Visitors(toSpill);
      for (auto inst : f->instructions) {
          inst->Accept(spill_visitor);
      }
  }
}