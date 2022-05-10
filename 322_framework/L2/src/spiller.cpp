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
    Spill_Visitors::Spill_Visitors(std::string toSpill, std::string prefix, int num_vars_spilled) {
        this->var = toSpill;
        this->prefix = prefix;
        this->num_vars_spilled = num_vars_spilled;
        // this->count = spill_count;
    }

    int Spill_Visitors::getCount() {
      return this->count;
    }

    std::vector<Instruction*> Spill_Visitors::getInstructions() {
      return this->new_inst;
    }

    Instruction* Spill_Visitors::loadvar() {
      this->didSpill = true;
      Register* reg = new Register(rsp);
      Number* offset = new Number(this->num_vars_spilled*8);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->prefix + std::to_string(this->count);
      // std::cout << "load " << var_name << std::endl;
      Variable* var = new Variable(var_name);
      var->isSpill = true;

      Instruction_assignment* load_inst = new Instruction_assignment(mem, var);
      return load_inst;
    }

    Instruction* Spill_Visitors::storevar() {
      this->didSpill = true;
      Register* reg = new Register(rsp);
      Number* offset = new Number(this->num_vars_spilled*8);
      Memory* mem = new Memory(reg, offset);

      std::string var_name = this->prefix + std::to_string(this->count);
      // std::cout << "store" << var_name << std::endl;
      Variable* var = new Variable(var_name);
      var->isSpill = true;

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

      // if (dynamic_cast<Memory*>(src) != nullptr) {
      //   auto mem_temp = dynamic_cast<Memory*>(src);
      //   auto mem_fields = mem_temp->get();
      //   auto rv_temp = std::get<0>(mem_fields);
      //   rv_temp = dynamic_cast<Variable*>(rv_temp);
      //   if (rv_temp != nullptr) {
      //     auto offset_temp = std::get<1>(mem_fields);
      //     if (rv_temp->toString() == this->var) {
      //       didRead = true;
      //       Variable* var = new Variable(var_name);
      //       var->isSpill = true;
      //       src = new Memory(var, offset_temp);
      //     }
      //   }
      // }
      // if (dynamic_cast<Memory*>(dst) != nullptr) {
      //   auto mem_temp = dynamic_cast<Memory*>(dst);
      //   auto mem_fields = mem_temp->get();
      //   auto rv_temp = std::get<0>(mem_fields);
      //   rv_temp = dynamic_cast<Variable*>(rv_temp);
      //   if (rv_temp != nullptr) {
      //     auto offset_temp = std::get<1>(mem_fields);
      //     if (rv_temp->toString() == this->var) {
      //       didRead = true;
      //       Variable* var = new Variable(var_name);
      //       var->isSpill = true;
      //       dst = new Memory(var, offset_temp);
      //     }
      //   }
      // }
      if (dynamic_cast<Variable*>(src) != nullptr) {
        // auto src_temp = dynamic_cast<Variable*>(src);
        if (src->toString() == this->var) {
          didRead = true;
          src = new Variable(var_name);
          src->isSpill = true;
        }
      }
      else if (dynamic_cast<Memory*>(src) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(src);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          src = new Memory(var, offset_temp);
          src->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(dst) != nullptr) {
        // auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst->toString() == this->var) {
        didWrite = true;
        dst = new Variable(var_name);
        dst->isSpill = true;
        }
      }
      else if (dynamic_cast<Memory*>(dst) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(dst);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          Variable* var = new Variable(var_name);
          dst = new Memory(var, offset_temp);
          dst->isSpill = true;
        }
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

      // if (dynamic_cast<Memory*>(src) != nullptr) {
      //   auto mem_temp = dynamic_cast<Memory*>(src);
      //   auto mem_fields = mem_temp->get();
      //   auto rv_temp = std::get<0>(mem_fields);
      //   // rv_temp = dynamic_cast<Variable*>(rv_temp);
      //   if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
      //     auto offset_temp = std::get<1>(mem_fields);
      //     if (rv_temp->toString() == this->var) {
      //       didRead = true;
      //       Variable* var = new Variable(var_name);
      //       var->isSpill = true;
      //       src = new Memory(var, offset_temp);
      //     }
      //   }
      // }
      // if (dynamic_cast<Memory*>(dst) != nullptr) {
      //   auto mem_temp = dynamic_cast<Memory*>(dst);
      //   auto mem_fields = mem_temp->get();
      //   auto rv_temp = std::get<0>(mem_fields);
      //   // rv_temp = dynamic_cast<Variable*>(rv_temp);
      //   if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
      //     auto offset_temp = std::get<1>(mem_fields);
      //     if (rv_temp->toString() == this->var) {
      //       didRead = true;
      //       Variable* var = new Variable(var_name);
      //       var->isSpill = true;
      //       dst = new Memory(var, offset_temp);
      //     }
      //   }
      // }
      if (dynamic_cast<Variable*>(src) != nullptr) {
        // auto src_temp = dynamic_cast<Variable*>(src);
        if (src->toString() == this->var) {
          didRead = true;
          src = new Variable(var_name);
          src->isSpill = true;
        }
      }
      else if (dynamic_cast<Memory*>(src) != nullptr) {
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
      if (dynamic_cast<Variable*>(dst) != nullptr) {
        // auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst->toString() == this->var) {
          didRead = true;
          didWrite = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
      }
      else if (dynamic_cast<Memory*>(dst) != nullptr) {
        auto mem_temp = dynamic_cast<Memory*>(dst);
        auto mem_fields = mem_temp->get();
        auto rv_temp = std::get<0>(mem_fields);
        auto offset_temp = std::get<1>(mem_fields);
        if (rv_temp->toString() == this->var) {
          didRead = true;
          // didWrite = true;
          Variable* var = new Variable(var_name);
          dst = new Memory(var, offset_temp);
        }
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

      if (dynamic_cast<Variable*>(dst) != nullptr) {
        // auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst->toString() == this->var) {
          didRead = true;
          didWrite = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
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

      if (dynamic_cast<Variable*>(src) != nullptr) {
        // auto src_temp = dynamic_cast<Variable*>(src);
        if (src->toString() == this->var) {
          didRead = true;
          src = new Variable(var_name);
          src->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(dst) != nullptr) {
        // auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst->toString() == this->var) {
          didWrite = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
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

      if (dynamic_cast<Variable*>(arg1) != nullptr) {
        auto arg1_temp = dynamic_cast<Variable*>(arg1);
        if (arg1_temp->toString() == this->var) {
          didRead = true;
          arg1 = new Variable(var_name);
          arg1->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(arg2) != nullptr) {
        auto arg2_temp = dynamic_cast<Variable*>(arg2);
        if (arg2_temp->toString() == this->var) {
          didRead = true;
          arg2 = new Variable(var_name);
          arg2->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(dst) != nullptr) {
        auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst_temp->toString() == this->var) {
          didWrite = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
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
      // bool didWrite = false;
      std::string var_name = this->prefix + std::to_string(this->count);

      if (dynamic_cast<Variable*>(arg1) != nullptr) {
        auto arg1_temp = dynamic_cast<Variable*>(arg1);
        if (arg1_temp->toString() == this->var) {
          didRead = true;
          arg1 = new Variable(var_name);
          arg1->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(arg2) != nullptr) {
        auto arg2_temp = dynamic_cast<Variable*>(arg2);
        if (arg2_temp->toString() == this->var) {
          didRead = true;
          arg2 = new Variable(var_name);
          arg2->isSpill = true;
        }
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_cjump(arg1, arg2, label, op);
      this->new_inst.push_back(inst);
      // if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead) this->count++;
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
      
      if (dynamic_cast<Variable*>(arg1) != nullptr) {
        auto arg1_temp = dynamic_cast<Variable*>(arg1);
        if (arg1_temp->toString() == this->var) {
          didRead = true;
          arg1 = new Variable(var_name);
          arg1->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(arg2) != nullptr) {
        auto arg2_temp = dynamic_cast<Variable*>(arg2);
        if (arg2_temp->toString() == this->var) {
          didRead = true;
          arg2 = new Variable(var_name);
          arg2->isSpill = true;
        }
      }
      if (dynamic_cast<Variable*>(dst) != nullptr) {
        auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst_temp->toString() == this->var) {
          didWrite = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
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

      if (dynamic_cast<Variable*>(u) != nullptr) {
        auto u_temp = dynamic_cast<Variable*>(u);
        if (u_temp->toString() == this->var) {
          didRead = true;
          u = new Variable(var_name);
          u->isSpill = true;
        }
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

      if (dynamic_cast<Variable*>(dst) != nullptr) {
        auto dst_temp = dynamic_cast<Variable*>(dst);
        if (dst_temp->toString() == this->var) {
          didRead = true;
          dst = new Variable(var_name);
          dst->isSpill = true;
        }
      }

      if (this->count != 0 && didRead) this->new_inst.push_back(this->loadvar());
      auto inst = new Instruction_stackarg(dst, M);
      this->new_inst.push_back(inst);
      if (didWrite) this->new_inst.push_back(this->storevar());
      if (didRead || didWrite) this->count++;
    }

  std::vector<Instruction*> spill_one_var(Function* f, std::string toSpill, std::string prefix, int num_vars_spilled) {
    // std::cout << "we made it to spill_one_var()" << std::endl;
    auto spill_visitor = new Spill_Visitors(toSpill, prefix, num_vars_spilled);
    spill_visitor->func_vars = f->func_vars;
    // std::cout << "visiting instructions" << std::endl;
    for (auto inst : f->instructions) {
      // std::cout << "visiting instructions" << std::endl;
      inst->Accept(spill_visitor);
    }

    if (spill_visitor->didSpill) {
      spill_visitor->num_vars_spilled++;
    }

    return spill_visitor->getInstructions();

    // start printing to cout
    // std::cout << "(" << f->name << "\n\t";
    // std::cout << std::to_string(f->arguments) << " " << std::to_string(spill_visitor->num_vars_spilled) << "\n";
    
    // for (auto i : spill_visitor->getInstructions()) {
    //   std::cout << "\t" << i->toString() << "\n";
    // }

    // std::cout << ")" << std::endl;

    // //* NEW STUFF
    // Function* spilled_func = new Function();
    // spilled_func->name = f->name;
    // spilled_func->arguments = f->arguments;
    // spilled_func->num_locals = spill_visitor->num_vars_spilled;
    // spilled_func->instructions = spill_visitor->getInstructions();
    // spilled_func->spill_count = spill_visitor->count;

    // return spilled_func;
  } 

  Function* spill_mult_var(Function* f, std::vector<Variable*> toSpill, std::string prefix) {
    // std::cout << "start spilling" << std::endl;
    // Function* temp_func = new Function();
    // temp_func->instructions = f->instructions;
    // int spill_count = 0;
    int num_vars_spilled = 0;
    // std::cout << "start spilling one" << std::endl;
    for (auto var : toSpill) {
      int inst_count = f->instructions.size();
      f->instructions = spill_one_var(f, var->toString(), prefix, num_vars_spilled);
      if (f->instructions.size() > inst_count) num_vars_spilled += 1;
        // spill_count = temp_func->spill_count;
      // }
    }

    f->num_locals += num_vars_spilled;
    // std::cout << "finished spilling one" << std::endl;

    // Function* spilled_func = new Function();
    // temp_func->name = f->name;
    // temp_func->arguments = f->arguments;
    // temp_func->num_locals = num_vars_spilled;
    // temp_func->instructions = f->instructions;
    // spilled_func->spill_count = temp_func->spill_count;
    // std::cout << "done spilling" << std::endl;

    return f;
  }

  Function* spill_all(Function* f, std::string prefix) {
    int num_vars_spilled = 0;

    for (auto var : f->func_vars) {
      int inst_count = f->instructions.size();
      f->instructions = spill_one_var(f, var->toString(), prefix, num_vars_spilled);
      if (f->instructions.size() > inst_count) num_vars_spilled += 1;
    }

    f->num_locals += num_vars_spilled;
    return f;
  }
}
