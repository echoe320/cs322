#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>
#include <L2.h>

// included libraries
#include <unordered_set>

using namespace std;

namespace L2 {
  void Gen_Kill_Visitors::VisitInstruction(const Instruction_ret *element){
    // gen = rax + callee save registers
    int callee_list[] = [6, 7, 8, 11, 12, 13, 14, 15];
    // reg callee_list[] = [rax, rbx, rbp, r12, r13, r14, r15];
    // for (int ii = 6; i < 14; i++) {
    //   Item *i;
    //   auto regi = new Register();
    //   i = &regi;
    //   i->r = static_cast<reg>(ii);
    //   element->reads.insert(i);
    // }
    for (auto count : callee_list) {
      Item *i;
      auto regi = new Register();
      i = &regi;
      i->r = (reg)count;
      element->reads.insert(i);
    }
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_assignment *element){
    //check if src is a variable/register
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Memory *>(element->src) != nullptr) {
      Item *i;
      auto regi = new Register(); //create new Register object with Memory's reg r field
      i = &regi;
      i->r = element->src->r;
      element->reads.insert(i);
    }
    //check if dst is a variable/register
    if (dynamic_cast<Variable *>(element->dst) != nullptr) element->writes.insert(element->dst);
    else if (dynamic_cast<Register *>(element->dst) != nullptr) element->writes.insert(element->dst);
    else if (dynamic_cast<Memory *>(element->dst) != nullptr) {
      Item *i;
      auto regi = new Register(); //create new Register object with Memory's reg r field
      i = &regi;
      i->r = element->dst->r;
      element->writes.insert(i);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_arithmetic *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_crement *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_shift *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_cmp *element){
    element->writes.insert(element->dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_cjump *element){
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_lea *element){
    element->writes.insert(element->dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_calls *element){
    // gen = u, args used
    if (dynamic_cast<Register *>(element->u) != nullptr) element->reads.insert(element->u);
    //element->reads.insert(element->u);
    
    int numArgs = element->N.get();
    for (int i = 0; i < numArgs; i++){
      Item *it;
      auto
      i->r = static_cast<reg>(ii);
    }

    // kill = caller save registers
    int caller_list[] = [0, 1, 2, 3];
    for (auto count : callee_list) {
      Item *i;
      auto regi = new Register();
      i = &regi;
      i->r = (reg)count;
      element->writes.insert(i);
    }
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_runtime *element){
    // gen = args used
    // kill = caller save registers

  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_label *element){
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_goto *element){
    element->reads.clear();
    element->writes.clear();
  }

  void Gen_Kill_Visitors::VisitInstruction(const Instruction_stackarg *element){
    element->writes.insert(element->dst);
  }

  void create_liveness_file(Program p) {

    //Initialize vectors
    vector<unordered_set<Item *>> GEN; //[numInstructions]; //* GEN[i] = all variables read by instruction i
    vector<unordered_set<Item *>> KILL; //[numInstructions]; //* KILL[i] = all variables written/defined by instruction i
    vector<unordered_set<Item *>> IN; //[numInstructions];
    vector<unordered_set<Item *>> OUT; //[numInstructions];

    //Gen and kill 
    auto *gen_kill_visitor = new Gen_Kill_Visitors();

    for (auto f : p.functions) {
      for (auto i : f->instructions) {
        i->Accept(gen_kill_visitor);
        GEN.push_back(i->reads); // not sure if arrow or dot accessor (should be arrow)
        KILL.push_back(i->writes);
      }
    }

    //Successor and predecessor
  }
}