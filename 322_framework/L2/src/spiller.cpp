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

    std::vector<std::pair<bool, bool>> Spill_Visitors::get_spill_use() {
      return this->RWflags;
    }

    Instruction* Spill_Visitors::loadvar() {
      Register* reg = new Register(rsp);
      Number* offset = new Number(0);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->var + std::to_string(this->count);
      Variable* var = new Variable(var_name);

      Instruction_assignment* load_inst = new Instruction_assignment(var, mem);
      return load_inst;
    }

    Instruction* Spill_Visitors::storevar() {
      Register* reg = new Register(rsp);
      Number* offset = new Number(0);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->var + std::to_string(this->count);
      Variable* var = new Variable(var_name);
      this->count++;

      Instruction_assignment* store_inst = new Instruction_assignment(mem, var);
      return store_inst;
    }
    
    void Spill_Visitors::VisitInstruction(Instruction_ret *element){
      std::pair<bool, bool> flag (false, false);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_assignment *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (src->toString() == this->var) didRead = true;
      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_arithmetic *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (src->toString() == this->var) didRead = true;
      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_crement *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (dst->toString() == this->var) {
        didRead = true;
        didWrite = true;
      }
      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_shift *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (src->toString() == this->var) didRead = true;
      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_cmp *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto arg1 = std::get<1>(fields);
      auto arg2 = std::get<2>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (arg1->toString() == this->var) didRead = true;
      if (arg2->toString() == this->var) didRead = true;
      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_cjump *element){
      auto fields = element->get();
      auto arg1 = std::get<0>(fields);
      auto arg2 = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (arg1->toString() == this->var) didRead = true;
      if (arg2->toString() == this->var) didRead = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_lea *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto arg1 = std::get<1>(fields);
      auto arg2 = std::get<2>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (arg1->toString() == this->var) didRead = true;
      if (arg2->toString() == this->var) didRead = true;
      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_calls *element){
      auto fields = element->get();
      auto u = std::get<0>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (u->toString() == this->var) didRead = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_runtime *element){
      std::pair<bool, bool> flag (false, false);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_label *element){
      std::pair<bool, bool> flag (false, false);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_goto *element){
      std::pair<bool, bool> flag (false, false);
      this->RWflags.push_back(flag);
    }

    void Spill_Visitors::VisitInstruction(Instruction_stackarg *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);

      bool didRead = false;
      bool didWrite = false;

      if (dst->toString() == this->var) didWrite = true;

      std::pair<bool, bool> flag (didRead, didWrite);
      this->RWflags.push_back(flag);
    }

  void spill_one_var(Function* f, std::string toSpill, std::string prefix) {
    auto spill_visitor = new Spill_Visitors(toSpill);
    for (auto inst : f->instructions) {
        inst->Accept(spill_visitor);
    }

    auto flags = spill_visitor->get_spill_use();
    std::vector<Instruction*> new_instructions;

    for (int ii = 0; ii < f->instructions.size(); ii++) {
      if (flags[ii].first) new_instructions.push_back(spill_visitor->loadvar());
      //todo create new instruction with spilled var and push_back
      if (flags[ii].second) new_instructions.push_back(spill_visitor->storevar());
    }

    // start printing to cout
    std::cout << "(" << f->name << "\n\t";
    std::cout << std::to_string(f->arguments) << " 1\n";
    
    for (auto i : new_instructions) {
      std::cout << "\t" << i->toString() << "\n";
    }

    std::cout << ")" << std::endl;
  }
}