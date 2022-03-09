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

bool shouldPrint = true;

namespace L2 {

  void Gen_Kill_Visitors::VisitInstruction(Instruction_ret *element){
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
    if (shouldPrint) std::cout << "visited Instruction_assignment" << "\n";
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
      auto mem_temp = dynamic_cast<Memory *>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      // auto reg_temp = dynamic_cast<Register *>(reg);
      // auto dst = std::get<1>(fields);
      // Memory* reg_obj = (Memory*)src;
      // std::cout << "hi there" << "\n";
      // reg reg_temp = reg_obj->get().first;
      // std::cout << "hi there" << "\n";
      // Register* regi = new Register(reg_temp); //create new Register object with Memory's reg r field
      std::cout << "AHHHHHHHHHHHHHHHHHHHHHHH MEMORY VISITED AHHHHHHHHHHHHHHHHH" << std::endl;
      if (rv_temp->toString() != "rsp") element->reads.insert(rv_temp);
    }
    //check if dst is a variable/register
    if (dynamic_cast<Variable *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Register *>(dst) != nullptr) element->writes.insert(dst);
    else if (dynamic_cast<Memory *>(dst) != nullptr) {
      // Memory* reg_obj = (Memory*) dst;
      // reg reg_temp = reg_obj->get().first;
      // Register* regi = new Register(reg_temp); //create new Register object with Memory's reg r field
      // element->writes.insert(regi);
      auto mem_temp = dynamic_cast<Memory *>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      element->reads.insert(rv_temp);
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
    auto fields = element->get();
    auto N = std::get<1>(fields);
    
    // gen = args used
    Number* num = (Number*) N;
    int numArgs = num->get(); // get the number field from N 
    // int numArgs = 1; // get number of args from N (same problem as accessing Memory reg r field (line 39, 48))
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
        f->GEN.push_back(i->reads);
        f->KILL.push_back(i->writes);
      }

    //Print gen & kill methods
      std::cout << "Gen:" << std::endl;
      int count = 0;
      for (auto i: f->GEN) {
        std::cout << std::to_string(count) << ": ";
        for(auto item: i) {
          if (dynamic_cast<Variable *>(item) != nullptr){
            Variable* var_temp = (Variable*)item;
            std::cout << var_temp->toString() << " ";
          } else if (dynamic_cast<Register *>(item) != nullptr) {
            Register* reg_temp = (Register*)item;
            std::cout << reg_temp->toString() << " ";
            // std::cout << get_enum_string(reg_temp->get()) << std::endl;
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
            // std::cout << get_enum_string(reg_temp->get()) << std::endl;
          }
        }
        count++;
        std::cout << std::endl;
      }
      // std::cout << std::endl;

      // Successor and predecessor
      std::cout << "Start of SuccessorsPredecessors" << std::endl;
      f->findSuccessorsPredecessors();
      std::cout << "End of SuccessorsPredecessors" << std::endl;
      // * UNCOMMENT TO PRINT SUCCESSORS
      std::set<int>::iterator it;
      for (auto f : p.functions) {
        for (int ii = 0; ii < f->instructions.size(); ii++) {
          
          std::cout << "instruction " << std::to_string(ii) << " succeeded by: ";
          for (it = f->instructions[ii]->successor_idx.begin(); it != f->instructions[ii]->successor_idx.end(); ++it)
            std::cout << ' ' << *it;
          std::cout << '\n';
        }
      }


      // Compute IN and OUT sets
      // std::cout << "Start of INOUTSETS" << std::endl;
      // <std::unordered_set<Item *>> temp_IN;
      // <std::unordered_set<Item *>> temp_OUT;

      bool didChange;
      do {
        didChange = false;
        for (int ii = 0; ii < f->instructions.size(); ii++) {
          std::unordered_set<Item *> temp_IN = f->instructions[ii]->IN; //set equal to instruction's IN set (this will be the previous iteration's) and compare at end
          std::unordered_set<Item *> temp_OUT = f->instructions[ii]->OUT;
          f->instructions[ii]->IN.clear();
          f->instructions[ii]->OUT.clear();

          // std::cout << "instruction #" << std::to_string(ii) << std::endl;

          // IN[i] = GEN[i] ∪(OUT[i] – KILL[i])
          // std::cout << "adding GEN to IN" << std::endl;

          if (!f->GEN[ii].empty()) {
            for (auto it = f->GEN[ii].begin(); it != f->GEN[ii].end(); ++it) {
              f->instructions[ii]->IN.insert(*it);
            }
          }
          // std::cout << "done adding GEN to IN" << std::endl;

          // std::cout << "populating instruction's OUT set" << std::endl;
          // find the successor(s) of the current instruction
          for (auto it1 = f->instructions[ii]->successor_idx.begin(); it1 != f->instructions[ii]->successor_idx.end(); ++it1) {
            // iterate through the successors' IN sets
            for (auto it2 = f->instructions[*it1]->IN.begin(); it2 != f->instructions[*it1]->IN.end(); ++it2) {
              // add each item in the successors' IN sets to the current instruction's OUT set
              f->instructions[ii]->OUT.insert(*it2);
            }
          }
          // std::cout << "done populating instruction's OUT set" << std::endl;
          
          bool OUTKILLFLAG;
          // std::cout << "Start of checking OUT and KILL" << std::endl;
          
          for (auto it1 = f->instructions[ii]->OUT.begin(); it1 != f->instructions[ii]->OUT.end(); ++it1) {
            // std::cout << "Start of checking indicies"  << std::endl;
            OUTKILLFLAG = false;
            for (auto it2 = f->KILL[ii].begin(); it2 != f->KILL[ii].end(); ++it2) {
              if (*it1 == *it2) {
                OUTKILLFLAG = true;
                break;
              }
            }
            if (!OUTKILLFLAG) f->instructions[ii]->IN.insert(*it1);
            // std::cout << "End of checking indicies"  << std::endl;
          }
          // std::cout << "END of checking OUT and KILL" << std::endl;
          
          if (temp_OUT != f->instructions[ii]->OUT || temp_IN != f->instructions[ii]->IN) {
            didChange = true;
          }
        }
      } while (didChange);

      // std::cout << "End of INOUTSETS" << std::endl;

      //* UNCOMMENT TO PRINT IN AND OUT SETS
      std::cout << "IN: " << std::endl;
      count = 0;
      for (int kk = 0; kk < f->instructions.size(); kk++) {
        std::cout << std::to_string(count) << ": ";
        for (auto it = f->instructions[kk]->IN.begin(); it != f->instructions[kk]->IN.end(); ++it) {
          if (dynamic_cast<Variable *>(*it) != nullptr) {
            Variable* var_temp = (Variable*) *it;
            std::cout << var_temp->toString() << " ";
          }
          else if (dynamic_cast<Register *>(*it) != nullptr) {
            Register* reg_temp = (Register*) *it;
            std::cout << reg_temp->toString() << " ";
          }
        }
        count++;
        std::cout << std::endl;
      }
      std::cout << "OUT: " << std::endl;
      count = 0;
      for (int kk = 0; kk < f->instructions.size(); kk++) {
        std::cout << std::to_string(count) << ": ";
        for (auto it = f->instructions[kk]->OUT.begin(); it != f->instructions[kk]->OUT.end(); ++it) {
          if (dynamic_cast<Variable *>(*it) != nullptr) {
            Variable* var_temp = (Variable*) *it;
            std::cout << var_temp->toString() << " ";
          }
          else if (dynamic_cast<Register *>(*it) != nullptr) {
            Register* reg_temp = (Register*) *it;
            std::cout << reg_temp->toString() << " ";
          }
        }
        count++;
        std::cout << std::endl;
      }
    //TODO: have another for loop to push_back the instruction's IN and OUT sets to the function's IN and OUT vectors (AFTER all the do-while)
    }
  }
}