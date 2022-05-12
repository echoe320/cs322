#include <iostream>
#include <string>
#include <unordered_map>

#include "L3.h"

using namespace std;

namespace L3 {
  /* helper functions */
  std::string get_reg_string (int enum_value) {
    return (reg_enum_str[enum_value]);
  }

  // Label Item
  Label::Label(string l) {
      this->labelname = l;
  } 
  string Label::get() {
    return this->labelname;
  }
  string Label::toString() {
    return this->labelname;
  }

  // Number Item
  Number::Number(int64_t n) {
    this->num = n;
  }
  int64_t Number::get() {
    return this->num;
  }
  string Number::toString() {
    return to_string(this->num);
  }

  // Variable Item
  Variable::Variable(string v) {
    this->variableName = v;
  }
  string Variable::get() {
    return this->variableName;
  }
  string Variable::toString() {
    return this->variableName;
  }
  // Operation Item
  Operation::Operation(string op) {
    this->op = op;
  }
  string Operation::get() {
    return this->op;
  }
  string Operation::toString() {
    return this->op;
  }
  String::String(std::string sName){
      this->sName = sName; 
  }
  string String::get() {
      return this->sName;
  }
  string String::toString() {
      return this->sName;
  }
  Empty::Empty() {}

  void Instruction_assignment::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_load::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_store::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_ret_not::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_ret_t::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_arithmetic::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_cmp::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_br_label::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_br_t::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_call_noassign::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_call_assignment::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }
  void Instruction_label::Accept(Visitor* v) {
    v->VisitInstruction(this);
  }

    //* FUNCTION - setting successors and predecessors
  void Function::findSuccessorsPredecessors() {
    int instructions_len = this->instructions.size();

    //* initialize label target map
    std::unordered_map<std::string, int> label_dict;
    for (int i = 0; i < instructions_len; i++) {
      if (dynamic_cast<Instruction_label *>(this->instructions[i]) != nullptr) {
        auto inst_temp = static_cast<Instruction_label *>(this->instructions[i]);
        auto label = inst_temp->toString();
        label_dict[label] = i;
      }
    }

    for (int ii = 0; ii < instructions_len; ii++) {
      this->instructions[ii]->successor_idx.clear();
      // this->instructions[ii]->prede_idx.clear();
      // return instruction has no successors
      if (dynamic_cast<Instruction_ret *>(this->instructions[ii]) != nullptr) {
        // if (doiprint) std::cout << "return succ" << std::endl;
        this->instructions[ii]->successor_idx.clear();
      }

      // call tensor_error has no successors
      // else if (dynamic_cast<Instruction_runtime *>(this->instructions[ii]) != nullptr) {
      //   if (doiprint) std::cout << "runtime succ" << std::endl;
      //   auto inst_temp = static_cast<Instruction_runtime *>(this->instructions[ii]);
      //   auto rt = static_cast<Runtime *>(std::get<0>(inst_temp->get()));
      //   auto rt_code = rt->get();
      //   if (rt_code == rt_tensor_error) continue;
      //   else this->instructions[ii]->successor_idx.insert(ii + 1);
      // }

      //cjump has two successors: target + instruction right after
      // else if (dynamic_cast<Instruction_cjump *>(this->instructions[ii]) != nullptr) {
      //   if (doiprint) std::cout << "cjump succ" << std::endl;
      //   this->instructions[ii]->successor_idx.insert(ii + 1);
      //   auto inst_temp = dynamic_cast<Instruction_cjump *>(this->instructions[ii]);
      //   auto target = static_cast<Label *>(std::get<2>(inst_temp->get()));
      //   this->instructions[ii]->successor_idx.insert(label_dict[target->toString()]);
      // }

      // goto instruction's successor is the target (label)
      // else if (dynamic_cast<Instruction_goto *>(this->instructions[ii]) != nullptr) {
      //   if (doiprint) std::cout << "goto succ" << std::endl;
      //   auto inst_temp = dynamic_cast<Instruction_goto *>(this->instructions[ii]);
      //   auto target = static_cast<Label *>(std::get<0>(inst_temp->get()));
      //   this->instructions[ii]->successor_idx.insert(label_dict[target->toString()]);
      // }

      // if not a special case, successor is the instruction right after
      else {
        // if (doiprint) std::cout << "everyday succ" << std::endl;
        this->instructions[ii]->successor_idx.insert(ii + 1);
      }
    }
  }

  //Program 
  Program::Program() {}
  //Function 
  Function::Function(void) {}

  Variable* Function::newVariable(std::string variable){
    std::string variableName = "%var_" + variable.substr(1);
    if(Function::variables.find(variableName) != Function::variables.end()){
      return Function::variables[variableName];
    }
    Function::variables[variableName] = new Variable(variableName);
    return Function::variables[variableName];
  }
}