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
    Spill_Visitors::Spill_Visitors(std::string toSpill, std::string prefix) {
        this->var = toSpill;
        this->prefix = prefix;
    }

    int Spill_Visitors::getCount() {
      return this->count;
    }

    std::vector<Instruction*> Spill_Visitors::getInstructions() {
      return this->new_inst;
    }

    // std::vector<std::pair<bool, bool>> Spill_Visitors::get_spill_use() {
    //   return this->RWflags;
    // }

    Instruction* Spill_Visitors::loadvar() {
      Register* reg = new Register(rsp);
      Number* offset = new Number(0);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->prefix + std::to_string(this->count);
      Variable* var = new Variable(var_name);

      Instruction_assignment* load_inst = new Instruction_assignment(mem, var);
      return load_inst;
    }

    Instruction* Spill_Visitors::storevar() {
      Register* reg = new Register(rsp);
      Number* offset = new Number(0);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->prefix + std::to_string(this->count);
      Variable* var = new Variable(var_name);

      Instruction_assignment* store_inst = new Instruction_assignment(var, mem);
      return store_inst;
    }
    
    void Spill_Visitors::VisitInstruction(Instruction_ret *element){
      this->new_inst.push_back(element);
    }

    void Spill_Visitors::VisitInstruction(Instruction_assignment *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (dynamic_cast<Memory*>(src) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(src);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          src = new Memory(var, offset_temp);
        }
      }
      if (dynamic_cast<Memory*>(dst) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(dst);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          dst = new Memory(var, offset_temp);
        }
      }
      if (src->toString() == this->var) {
        didRead = true;
        src = new Variable(var_name);
      }
      if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_assignment(src, dst);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_arithmetic *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);
      auto op = std::get<2>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (dynamic_cast<Memory*>(src) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(src);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          src = new Memory(var, offset_temp);
        }
      }
      if (dynamic_cast<Memory*>(dst) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(dst);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          dst = new Memory(var, offset_temp);
        }
      }
      if (src->toString() == this->var) {
        didRead = true;
        src = new Variable(var_name);
      }
      if (dst->toString() == this->var) {
        didRead = true;
        didWrite = true;
        dst = new Variable(var_name);
      }
      
      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_arithmetic(src, dst, op);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_crement *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto op = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (dst->toString() == this->var) {
        didRead = true;
        didWrite = true;
        dst = new Variable(var_name);
      }
      
      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_crement(dst, op);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_shift *element){
      auto fields = element->get();
      auto src = std::get<0>(fields);
      auto dst = std::get<1>(fields);
      auto op = std::get<2>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (src->toString() == this->var) {
        didRead = true;
        src = new Variable(var_name);
      }
      if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_shift(src, dst, op);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_cmp *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto arg1 = std::get<1>(fields);
      auto arg2 = std::get<2>(fields);
      auto op = std::get<3>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (arg1->toString() == this->var) {
        didRead = true;
        arg1 = new Variable(var_name);
      }
      if (arg2->toString() == this->var) {
        didRead = true;
        arg2 = new Variable(var_name);
      }
      if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_cmp(dst, arg1, arg2, op);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_cjump *element){
      auto fields = element->get();
      auto arg1 = std::get<0>(fields);
      auto arg2 = std::get<1>(fields);
      auto label = std::get<2>(fields);
      auto op = std::get<3>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (arg1->toString() == this->var) {
        didRead = true;
        arg1 = new Variable(var_name);
      }
      if (arg2->toString() == this->var) {
        didRead = true;
        arg2 = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_cjump(arg1, arg2, label, op);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_lea *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto arg1 = std::get<1>(fields);
      auto arg2 = std::get<2>(fields);
      auto mult = std::get<3>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (arg1->toString() == this->var) {
        didRead = true;
        arg1 = new Variable(var_name);
      }
      if (arg2->toString() == this->var) {
        didRead = true;
        arg2 = new Variable(var_name);
      }
      if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_lea(dst, arg1, arg2, mult);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_calls *element){
      auto fields = element->get();
      auto u = std::get<0>(fields);
      auto N = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (u->toString() == this->var) {
        didRead = true;
        u = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_calls(u, N);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

    void Spill_Visitors::VisitInstruction(Instruction_runtime *element){
      this->new_inst.push_back(element);
    }

    void Spill_Visitors::VisitInstruction(Instruction_label *element){
      this->new_inst.push_back(element);
    }

    void Spill_Visitors::VisitInstruction(Instruction_goto *element){
      this->new_inst.push_back(element);
    }

    void Spill_Visitors::VisitInstruction(Instruction_stackarg *element){
      auto fields = element->get();
      auto dst = std::get<0>(fields);
      auto M = std::get<1>(fields);

      bool didRead = false;
      bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_stackarg(dst, M);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

  void spill_one_var(Function* f, std::string toSpill, std::string prefix) {
    // instead of returning a new function for each spilled variable, we can just edit the instructions for the input function directly
    auto spill_visitor = new Spill_Visitors(toSpill, prefix);
    for (auto inst : f->instructions) {
        inst->Accept(spill_visitor);
    }

    if (spill_visitor->getCount() != 0) spill_visitor->numSpilled++;

    // start printing to cout
    std::cout << "(" << f->name << "\n\t";
    std::cout << std::to_string(f->arguments) << " " << std::to_string(spill_visitor->numSpilled) << "\n";
    
    for (auto i : spill_visitor->getInstructions()) {
      std::cout << "\t" << i->toString() << "\n";
    }

    std::cout << ")" << std::endl;
  }

  // Function* smill_mult_var(Function*f, std::vector<Variable*> toSpill, std::string prefix) {
  //   something idk
  // }
}