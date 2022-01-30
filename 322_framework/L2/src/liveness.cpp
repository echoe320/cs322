#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>
#include <L2.h>

// included libraries
#include <unordered_set>

using namespace std;

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
    //check if src is a variable/register
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Memory *>(element->src) != nullptr) {
      Item *i;
      Register regi(rdi); //create new Register object with Memory's reg r field
      i = &regi;
      // i->r = element->src->r;
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

  void Gen_Kill_Visitors::VisitInstruction(Instruction_arithmetic *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_crement *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_shift *element){
    element->reads.insert(element->dst);
    element->writes.insert(element->dst);
    if (dynamic_cast<Variable *>(element->src) != nullptr) element->reads.insert(element->src);
    else if (dynamic_cast<Register *>(element->src) != nullptr) element->reads.insert(element->src);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_cmp *element){
    element->writes.insert(element->dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_cjump *element){
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_lea *element){
    element->writes.insert(element->dst);
    // check if arg1 is variable
    if (dynamic_cast<Variable *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    else if (dynamic_cast<Register *>(element->arg1) != nullptr) element->reads.insert(element->arg1);
    // check if arg2 is variable
    if (dynamic_cast<Variable *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
    else if (dynamic_cast<Register *>(element->arg2) != nullptr) element->reads.insert(element->arg2);
  }

  void Gen_Kill_Visitors::VisitInstruction(Instruction_calls *element){
    // gen = u, args used
    if (dynamic_cast<Register *>(element->u) != nullptr) element->reads.insert(element->u);
    //element->reads.insert(element->u);
    
    //adding 
    int numArgs = element->N.get();
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
    // gen = args used
    int numArgs = element->N.get(); // get number of args
    for (int i = 0; i < numArgs; i++){
      Item* it;
      int arg_reg = arg_reg_list[i];
      Register regi(static_cast<reg>(arg_reg));
      it = &regi;
      element->reads.insert(it);
    }
    // kill = caller save registers

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