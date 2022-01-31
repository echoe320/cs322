#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>
// #include <L2.h>

// included libraries
#include <unordered_set>
#include <tuple>

namespace L2 {
  void Gen_Kill_Visitors::VisitInstruction(Instruction_ret *element){
    // gen = rax + callee save registers
    for (auto count : callee_reg_list) {
      Item *i;
      Register regi(static_cast<reg>(count));
      i = &regi;
      element->reads.insert(i);
    }
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_assignment *element){
    // init variables to check
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    //if src is a variable/register, add to GEN (aka reads)
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Memory *>(src) != nullptr) {
      Item *i;
      Memory* reg_obj = (Memory*)src;
      reg reg_temp = reg_obj->get().first;
      Register regi(rdi); //create new Register object with Memory's reg r field
      i = &regi;
      element->reads.insert(i);
    }
    //check if dst is a variable/register
    if (dynamic_cast<Variable *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Register *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Memory *>(dst) != nullptr) {
      Item *i;
      Memory* reg_obj = (Memory*)dst;
      reg reg_temp = reg_obj->get().first;
      Register regi(reg_temp); //create new Register object with Memory's reg r field
      i = &regi;
      element->writes.insert(i);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_arithmetic *element){
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) element->reads.insert(src);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_crement *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_shift *element){
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) element->reads.insert(src);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_cmp *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto arg1 = std::get<1>(fields);
    auto arg2 = std::get<2>(fields);

    element->writes.insert(dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(arg1) != nullptr) element->reads.insert(arg1);
    else if (dynamic_cast<Register *>(arg1) != nullptr) element->reads.insert(arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(arg2) != nullptr) {
      element->reads.insert(arg2);
    } else if (dynamic_cast<Register *>(arg2) != nullptr) {
      element->reads.insert(arg2);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_cjump *element){
    auto fields = element->get();
    auto arg1 = std::get<0>(fields);
    auto arg2 = std::get<1>(fields);

    // check if arg1 is variable
    if (dynamic_cast<Variable *>(arg1) != nullptr) element->reads.insert(arg1);
    else if (dynamic_cast<Register *>(arg1) != nullptr) element->reads.insert(arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(arg2) != nullptr) element->reads.insert(arg2);
    else if (dynamic_cast<Register *>(arg2) != nullptr) element->reads.insert(arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_lea *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto arg1 = std::get<1>(fields);
    auto arg2 = std::get<2>(fields);

    element->writes.insert(dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(arg1) != nullptr) element->reads.insert(arg1);
    else if (dynamic_cast<Register *>(arg1) != nullptr) element->reads.insert(arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(arg2) != nullptr) element->reads.insert(arg2);
    else if (dynamic_cast<Register *>(arg2) != nullptr) element->reads.insert(arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_calls *element){
    auto fields = element->get();
    auto u = std::get<0>(fields);
    auto N = std::get<1>(fields);
    
    // gen = u, args used
    if (dynamic_cast<Register *>(u) != nullptr) element->reads.insert(u);
    
    //adding 
    Number* num = (Number*) N;
    int numArgs = num->get(); // get the number field from N 
    // auto numArgs = 1; // get number of args from N (same problem as accessing Memory reg r field (line 39, 48))
    for (int i = 0; i < numArgs; i++){
      Item* it;
      int arg_reg = arg_reg_list[i];
      Register regi(static_cast<reg>(arg_reg));
      it = &regi;
      element->reads.insert(it);
    }

    // kill = caller save registers
    for (auto count : caller_reg_list) {
      Item *i;
      Register regi(static_cast<reg>(count));
      i = &regi;
      element->writes.insert(i);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_runtime *element){
    auto fields = element->get();
    auto N = std::get<1>(fields);
    
    // gen = args used
    Number* num = (Number*) N;
    int numArgs = num->get(); // get the number field from N 
    // int numArgs = 1; // get number of args from N (same problem as accessing Memory reg r field (line 39, 48))
    for (int i = 0; i < numArgs; i++){
      Item* it;
      int arg_reg = arg_reg_list[i];
      Register regi(static_cast<reg>(arg_reg));
      it = &regi;
      element->reads.insert(it);
    }
    // kill = caller save registers
    for (auto count : caller_reg_list) {
      Item *i;
      Register regi(static_cast<reg>(count));
      i = &regi;
      element->writes.insert(i);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_label *element){
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_goto *element){
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_stackarg *element){
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    element->writes.insert(dst);
  }

void create_liveness_list(Program p) { 

    //Initialize vectors
    std::vector<std::unordered_set<Item *>> GEN; //[numInstructions]; //* GEN[i] = all variables read by instruction i
    std::vector<std::unordered_set<Item *>> KILL; //[numInstructions]; //* KILL[i] = all variables written/defined by instruction i
    std::vector<std::unordered_set<Item *>> IN; //[numInstructions];
    std::vector<std::unordered_set<Item *>> OUT; //[numInstructions];

    //Gen and kill 
    auto *gen_kill_visitor = new Gen_Kill_Visitors();

    for (auto f : p.functions) {
      for (auto i : f->instructions) {
        i->Accept(gen_kill_visitor);
        GEN.push_back(i->reads); // not sure if arrow or dot accessor (should be arrow)
        KILL.push_back(i->writes);
      }
    }

    //Print gen & kill methods
    for (auto i: GEN) {
      for(auto item: i){
        if (dynamic_cast<Variable *>(item) != nullptr){
          Variable* var_temp = (Variable*)item;
          std::cout >> var_temp->get() >> std::endl;
        } else if (dynamic_cast<Register *>(item) != nullptr) {
          Register* reg_temp = (Register*)item;
          std::cout >> get_enum_string(reg_temp->get()) >> std::endl;
        }
      }
    }


    //Successor and predecessor
  }
}