#include <string>
#include <iostream>
#include <fstream>

// #include <L2.h>
#include <liveness.h>
#include <L2_parser.h>

// included libraries
#include <unordered_set>
#include <tuple>

int caller_reg_list[] = {L2::reg::r10, L2::reg::r11, L2::reg::r8, L2::reg::r9, L2::reg::rax, L2::reg::rcx, L2::reg::rdi, L2::reg::rdx, L2::reg::rsi};
int callee_reg_list[] = {L2::reg::r12, L2::reg::r13, L2::reg::r14, L2::reg::r15, L2::reg::rbp, L2::reg::rbx};
int arg_reg_list[] = {L2::reg::rdi, L2::reg::rsi, L2::reg::rdx, L2::reg::rcx, L2::reg::r8, L2::reg::r9};

namespace L2 {

  void Gen_Kill_Visitors::VisitInstruction(Instruction_ret *element){
    // gen = rax + callee save registers
    for (auto count : callee_reg_list) {
      Register* regi = new Register(static_cast<reg>(count));
      // std::cout << regi->toString() << "\n";
      element->reads.insert(regi);
    }
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_assignment *element){
    std::cout << "visited Instruction_assignment" << "\n";
    // init variables to check
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    // auto s = std::get<0>(fields);

    // auto dst_1 = element -> get_dst();
    // std::cout << dst << "\n";
    // std::cout << "hi there" << "\n";

    // auto var_temp = dynamic_cast<Number *>(src); // (Variable*)item
    // auto var_temp = dynamic_cast<Register *>(dst);
    // Register* reg_obj = (Register*)src_1;
    // std::cout << "casted" << "\n";
    // std::cout << var_temp->toString() << "\n";

    //if src is a variable/register, add to GEN (aka reads)
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) {
      // std::cout << "successfully casted" << "\n";
      element->reads.insert(src);
      // auto var_temp = dynamic_cast<Register *>(src);
      // std::cout << var_temp->toString() << "\n";
    }
    else if (dynamic_cast<Memory *>(src) != nullptr) {
      Memory* reg_obj = (Memory*)src;
      reg reg_temp = reg_obj->get().first;
      Register* regi = new Register(rdi); //create new Register object with Memory's reg r field
      element->reads.insert(regi);
    }
    //check if dst is a variable/register
    if (dynamic_cast<Variable *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Register *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Memory *>(dst) != nullptr) {
      Memory* reg_obj = (Memory*)dst;
      reg reg_temp = reg_obj->get().first;
      Register* regi = new Register(reg_temp); //create new Register object with Memory's reg r field
      element->writes.insert(regi);
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
    //Gen and kill 
    auto gen_kill_visitor = new Gen_Kill_Visitors();
    std::cout << "gen_kill_visitor initialized" << "\n";

    for (auto f : p.functions) {
      for (auto i : f->instructions) {
        std::cout << i->typeAsString() << "\n";
        i->Accept(gen_kill_visitor);
        f->GEN.push_back(i->reads); // not sure if arrow or dot accessor (should be arrow)
        f->KILL.push_back(i->writes);
      }

    //Print gen & kill methods
      std::cout << "Gen: ";
      for (auto i: f->GEN) {
        for(auto item: i){
          if (dynamic_cast<Variable *>(item) != nullptr){
            Variable* var_temp = (Variable*)item;
            std::cout << var_temp->toString() << " ";
          } else if (dynamic_cast<Register *>(item) != nullptr) {
            Register* reg_temp = (Register*)item;
            std::cout << reg_temp->toString() << " ";
            // std::cout << get_enum_string(reg_temp->get()) << std::endl;
          }
        } 
      }
      std::cout << std::endl;

      std::cout << "Kill: ";
      for (auto i: f->KILL) {
        for(auto item: i){
          if (dynamic_cast<Variable *>(item) != nullptr){
            Variable* var_temp = (Variable*)item;
            std::cout << var_temp->toString() << " ";
          } else if (dynamic_cast<Register *>(item) != nullptr) {
            Register* reg_temp = (Register*)item;
            std::cout << reg_temp->toString() << " ";
            // std::cout << get_enum_string(reg_temp->get()) << std::endl;
          }
        }
      }
      std::cout << std::endl;

      // Successor and predecessor
      for (auto f : p.functions) {
        f->findSuccessorsPredecessors();
      }

      // Compute IN and OUT sets
      bool didChange = false;
      do {
        continue;
      } while (didChange);
    }
  }
}