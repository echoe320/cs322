#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <color_visitor.h>
// #include <L2_parser.h>

// included libraries
// #include <unordered_set>
// #include <tuple>


namespace L2 {
  Color_Visitors::Color_Visitors() {
      // this->var = toSpill;
      // this->prefix = prefix;
      // this->num_vars_spilled = num_vars_spilled;
      // this->count = spill_count;
  }
  
  void Color_Visitors::VisitInstruction(Instruction_ret *element){
    this->new_inst.push_back(element);
  }

  void Color_Visitors::VisitInstruction(Instruction_assignment *element){
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto src_node = lookupNode(src->toString());
      if (src_node->color != "spill") {
        src = new Register(color_dict[src_node->color]);
      }
    }
    else if (dynamic_cast<Memory*>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory*>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      auto offset_temp = std::get<1>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
        auto src_node = lookupNode(rv_temp->toString());
        if (src_node->color != "spill") {
          Register* regi = new Register(color_dict[src_node->color]);
          src = new Memory(regi, offset_temp);
        }
      }
    }
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }
    else if (dynamic_cast<Memory*>(dst) != nullptr) {
      auto mem_temp = dynamic_cast<Memory*>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      auto offset_temp = std::get<1>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
        auto dst_node = lookupNode(rv_temp->toString());
        if (dst_node->color != "spill") {
          Register* regi = new Register(color_dict[dst_node->color]);
          dst = new Memory(regi, offset_temp);
        }
      }
    }

    auto inst = new Instruction_assignment(src, dst);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_arithmetic *element){
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    auto op = std::get<2>(fields);

    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto src_node = lookupNode(src->toString());
      if (src_node->color != "spill") {
        src = new Register(color_dict[src_node->color]);
      }
    }
    else if (dynamic_cast<Memory*>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory*>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      auto offset_temp = std::get<1>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
        auto src_node = lookupNode(rv_temp->toString());
        if (src_node->color != "spill") {
          Register* regi = new Register(color_dict[src_node->color]);
          src = new Memory(regi, offset_temp);
        }
      }
    }
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }
    else if (dynamic_cast<Memory*>(dst) != nullptr) {
      auto mem_temp = dynamic_cast<Memory*>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      auto offset_temp = std::get<1>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr) {
        auto dst_node = lookupNode(rv_temp->toString());
        if (dst_node->color != "spill") {
          Register* regi = new Register(color_dict[dst_node->color]);
          dst = new Memory(regi, offset_temp);
        }
      }
    }
    
    auto inst = new Instruction_arithmetic(src, dst, op);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_crement *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto op = std::get<1>(fields);

    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }
    
    auto inst = new Instruction_crement(dst, op);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_shift *element){
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    auto op = std::get<2>(fields);

    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto src_node = lookupNode(src->toString());
      if (src_node->color != "spill") {
        src = new Register(color_dict[src_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }

    auto inst = new Instruction_shift(src, dst, op);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_cmp *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto arg1 = std::get<1>(fields);
    auto arg2 = std::get<2>(fields);
    auto op = std::get<3>(fields);

    if (dynamic_cast<Variable*>(arg1) != nullptr) {
      auto arg1_node = lookupNode(arg1->toString());
      if (arg1_node->color != "spill") {
        arg1 = new Register(color_dict[arg1_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(arg2) != nullptr) {
      auto arg2_node = lookupNode(arg2->toString());
      if (arg2_node->color != "spill") {
        arg2 = new Register(color_dict[arg2_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }

    auto inst = new Instruction_cmp(dst, arg1, arg2, op);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_cjump *element){
    auto fields = element->get();
    auto arg1 = std::get<0>(fields);
    auto arg2 = std::get<1>(fields);
    auto label = std::get<2>(fields);
    auto op = std::get<3>(fields);

    if (dynamic_cast<Variable*>(arg1) != nullptr) {
      auto arg1_node = lookupNode(arg1->toString());
      if (arg1_node->color != "spill") {
        arg1 = new Register(color_dict[arg1_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(arg2) != nullptr) {
      auto arg2_node = lookupNode(arg2->toString());
      if (arg2_node->color != "spill") {
        arg2 = new Register(color_dict[arg2_node->color]);
      }
    }

    auto inst = new Instruction_cjump(arg1, arg2, label, op);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_lea *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto arg1 = std::get<1>(fields);
    auto arg2 = std::get<2>(fields);
    auto mult = std::get<3>(fields);
    
    if (dynamic_cast<Variable*>(arg1) != nullptr) {
      auto arg1_node = lookupNode(arg1->toString());
      if (arg1_node->color != "spill") {
        arg1 = new Register(color_dict[arg1_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(arg2) != nullptr) {
      auto arg2_node = lookupNode(arg2->toString());
      if (arg2_node->color != "spill") {
        arg2 = new Register(color_dict[arg2_node->color]);
      }
    }
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }

    auto inst = new Instruction_lea(dst, arg1, arg2, mult);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_calls *element){
    auto fields = element->get();
    auto u = std::get<0>(fields);
    auto N = std::get<1>(fields);

    if (dynamic_cast<Variable*>(u) != nullptr) {
      auto u_node = lookupNode(dst->toString());
      if (u_node->color != "spill") {
        u = new Register(color_dict[u_node->color]);
      }
    }

    auto inst = new Instruction_calls(u, N);
    this->new_inst.push_back(inst);
  }

  void Color_Visitors::VisitInstruction(Instruction_runtime *element){
    this->new_inst.push_back(element);
  }

  void Color_Visitors::VisitInstruction(Instruction_label *element){
    this->new_inst.push_back(element);
  }

  void Color_Visitors::VisitInstruction(Instruction_goto *element){
    this->new_inst.push_back(element);
  }

  void Color_Visitors::VisitInstruction(Instruction_stackarg *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto M = std::get<1>(fields);

    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto dst_node = lookupNode(dst->toString());
      if (dst_node->color != "spill") {
        dst = new Register(color_dict[dst_node->color]);
      }
    }

    auto inst = new Instruction_stackarg(dst, M);
    this->new_inst.push_back(inst);
  }

  Function* color2reg(Function* f) {
    auto color_visitor = new Color_Visitors();
    for (auto inst : f->instructions) {
      inst->Accept(color_visitor);
    }

    f->instructions = color_visitor->new_inst;
    return f;
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
  }
}
