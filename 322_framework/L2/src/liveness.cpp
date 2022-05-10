#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// #include <L2.h>
#include <liveness.h>
#include <L2_parser.h>

// included libraries
#include <unordered_set>
#include <tuple>

int caller_reg_list[] = {L2::reg::r10, L2::reg::r11, L2::reg::r8, L2::reg::r9, L2::reg::rax, L2::reg::rcx, L2::reg::rdi, L2::reg::rdx, L2::reg::rsi};
int callee_reg_list[] = {L2::reg::r12, L2::reg::r13, L2::reg::r14, L2::reg::r15, L2::reg::rbp, L2::reg::rbx};
int arg_reg_list[] = {L2::reg::rdi, L2::reg::rsi, L2::reg::rdx, L2::reg::rcx, L2::reg::r8, L2::reg::r9};

bool shouldPrint = false;

namespace L2 {

  void Gen_Kill_Visitors::VisitInstruction(Instruction_ret *element){
    if (shouldPrint) std::cout << "visited return instruction" << "\n";
    // gen = rax + callee save registers
    for (auto count : callee_reg_list) {
      Register* regi = new Register(static_cast<reg>(count));
      // if (shouldPrint) std::cout << regi->toString() << "\n";
      element->reads.insert(regi);
    }
    // rax
    Register* regi = new Register(rax);
    element->reads.insert(regi);

    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_assignment *element){
    if (shouldPrint) std::cout << "visited assignment instruction" << "\n";
    // init variables to check
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    //if src is a variable/register, add to GEN (aka reads)
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) {
      element->reads.insert(src);
    }
    else if (dynamic_cast<Memory *>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (rv_temp->toString() != "rsp") element->reads.insert(rv_temp);
    }
    //check if dst is a variable/register
    if (dynamic_cast<Variable *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Register *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Memory *>(dst) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (rv_temp->toString() != "rsp") element->reads.insert(rv_temp);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_arithmetic *element){
    if (shouldPrint) std::cout << "visited arithmetic instruction" << "\n";
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Memory *>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (rv_temp->toString() != "rsp") element->reads.insert(rv_temp);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_crement *element){
    if (shouldPrint) std::cout << "visited crement instruction" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_shift *element){
    if (shouldPrint) std::cout << "visited shift instruction" << "\n";
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);

    element->reads.insert(dst);
    element->writes.insert(dst);
    if (dynamic_cast<Variable *>(src) != nullptr) element->reads.insert(src);
    else if (dynamic_cast<Register *>(src) != nullptr) element->reads.insert(src);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_cmp *element){
    if (shouldPrint) std::cout << "visited compare instruction" << "\n";
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
    if (shouldPrint) std::cout << "visited conditional jump instruction" << "\n";
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
    if (shouldPrint) std::cout << "visited lea (@) instruction" << "\n";
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
    if (shouldPrint) std::cout << "visited function call instruction" << "\n";
    auto fields = element->get();
    auto u = std::get<0>(fields);
    auto N = std::get<1>(fields);
    
    // gen = u, args used
    if (dynamic_cast<Register *>(u) != nullptr) element->reads.insert(u);
    else if (dynamic_cast<Variable *>(u) != nullptr) element->reads.insert(u);
    
    //adding 
    Number* num = (Number*) N;
    int numArgs = num->get(); // get the number field from N 
    for (int i = 0; i < numArgs; i++){
      int arg_reg = arg_reg_list[i];
      Register* regi = new Register(static_cast<reg>(arg_reg));
      element->reads.insert(regi);
    }

    // kill = caller save registers
    for (auto count : caller_reg_list) {
      Register* regi = new Register(static_cast<reg>(count));
      element->writes.insert(regi);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_runtime *element){
    if (shouldPrint) std::cout << "visited runtime call instruction" << "\n";
    auto fields = element->get();
    auto N = std::get<1>(fields);
    
    // gen = args used
    Number* num = (Number*) N;
    int numArgs = num->get(); // get the number field from N 
    for (int i = 0; i < numArgs; i++){
      int arg_reg = arg_reg_list[i];
      Register* regi = new Register(static_cast<reg>(arg_reg));
      element->reads.insert(regi);
    }
    // kill = caller save registers
    for (auto count : caller_reg_list) {
      Register* regi = new Register(static_cast<reg>(count));
      element->writes.insert(regi);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_label *element){
    if (shouldPrint) std::cout << "visited label instruction" << "\n";
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_goto *element){
    if (shouldPrint) std::cout << "visited goto instruction" << "\n";
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_stackarg *element){
    if (shouldPrint) std::cout << "visited stack arg instruction" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    element->writes.insert(dst);
  }

  void create_liveness_list(Function* f) {
    //Gen and kill 
    auto gen_kill_visitor = new Gen_Kill_Visitors();
    for (auto i : f->instructions) {
      i->Accept(gen_kill_visitor);
      f->GEN.push_back(i->reads);
      f->KILL.push_back(i->writes);
    }

    int count = 0;
    //Print gen & kill methods
    if (shouldPrint) {
      std::cout << "Gen:" << std::endl;
      for (auto i: f->GEN) {
        std::cout << std::to_string(count) << ": ";
        for(auto item: i) {
          if (dynamic_cast<Variable *>(item) != nullptr){
            Variable* var_temp = (Variable*)item;
            std::cout << var_temp->toString() << " ";
          } else if (dynamic_cast<Register *>(item) != nullptr) {
            Register* reg_temp = (Register*)item;
            std::cout << reg_temp->toString() << " ";
          }
        } 
        count++;
        std::cout << std::endl;
      }

      std::cout << "Kill: " << std::endl;
      count = 0;
      for (auto i: f->KILL) {
        std::cout << std::to_string(count) << ": ";
        for(auto item: i){
          if (dynamic_cast<Variable *>(item) != nullptr){
            Variable* var_temp = (Variable*)item;
            std::cout << var_temp->toString() << " ";
          } else if (dynamic_cast<Register *>(item) != nullptr) {
            Register* reg_temp = (Register*)item;
            std::cout << reg_temp->toString() << " ";
          }
        }
        count++;
        std::cout << std::endl;
      }
    }

    // Successor and predecessor
    f->findSuccessorsPredecessors();
    if (shouldPrint) {
      for (int ii = 0; ii < f->instructions.size(); ii++) {
        std::cout << "instruction " << std::to_string(ii) << " succeeded by: ";
        for (auto it = f->instructions[ii]->successor_idx.begin(); it != f->instructions[ii]->successor_idx.end(); ++it)
          std::cout << ' ' << *it;
        std::cout << '\n';
      }
    }


    // Compute IN and OUT sets
    bool didChange;
    do {
      didChange = false;
      for (int ii = f->instructions.size() - 1; ii >= 0; ii--) {
        std::unordered_set<Item *> temp_IN = f->instructions[ii]->IN; //set equal to instruction's IN set (this will be the previous iteration's) and compare at end
        std::unordered_set<Item *> temp_OUT = f->instructions[ii]->OUT;
        f->instructions[ii]->IN.clear();
        f->instructions[ii]->OUT.clear();

        //* populate OUT sets
        if (shouldPrint) std::cout << "populating instruction's OUT set" << std::endl;
        bool dupie;
        // find the successor(s) of the current instruction
        for (auto it1 = f->instructions[ii]->successor_idx.begin(); it1 != f->instructions[ii]->successor_idx.end(); ++it1) {
          // iterate through the successors' IN sets
          for (auto it2 = f->instructions[*it1]->IN.begin(); it2 != f->instructions[*it1]->IN.end(); ++it2) {
            auto temp2 = *it2;
            dupie = false;
            // add each item in the successors' IN sets to the current instruction's OUT set
            for (auto it3 = f->instructions[ii]->OUT.begin(); it3 != f->instructions[ii]->OUT.end(); ++it3) {
              auto temp3 = *it3;
              if (temp2->toString() == temp3->toString()) {
                dupie = true;
                break;
              }
            }
            if (!dupie) f->instructions[ii]->OUT.insert(temp2);
          }
        }

        //! START
        //* GEN[ii]
        if (shouldPrint) std::cout << "adding GEN to IN" << std::endl;
        f->instructions[ii]->IN = f->GEN[ii];
        if (shouldPrint) std::cout << "done adding GEN to IN" << std::endl;

        //* OUT[ii] - KILL[ii]
        bool OUTKILLFLAG;
        for (auto it1 = f->instructions[ii]->OUT.begin(); it1 != f->instructions[ii]->OUT.end(); ++it1) {
          OUTKILLFLAG = false;
          auto temp1 = *it1;
          for (auto it2 = f->KILL[ii].begin(); it2 != f->KILL[ii].end(); ++it2) {
            auto temp2 = *it2;
            if (temp1->toString() == temp2->toString()) {
              OUTKILLFLAG = true;
              break;
            }
          }
          for (auto it = f->GEN[ii].begin(); it != f->GEN[ii].end(); ++it) {
            auto temp = *it;
            if (temp->toString() == temp1->toString()) {
              OUTKILLFLAG = true;
              break;
            }
          }
          if (!OUTKILLFLAG) f->instructions[ii]->IN.insert(*it1);
        }
        //! END
        
        if (temp_OUT != f->instructions[ii]->OUT || temp_IN != f->instructions[ii]->IN) {
          didChange = true;
        }
      }
    } while (didChange);

    for (auto i : f->instructions) {
      f->IN.push_back(i->IN);
      f->OUT.push_back(i->OUT);
    }
  }
}