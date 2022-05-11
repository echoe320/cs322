#include "L3.h"
#include <iostream>
#include <string>

using namespace std;

namespace L3
{
  /* helper functions */
  std::string get_reg_string (int enum_value) {
    return (reg_enum_str[enum_value]);
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

  // Label Item
  Label::Label(string l){
      this->labelname = l;
  } 
  string Label::get()
  {
    return this->labelname;
  }
  bool Label::operator==(const Label &other)
  {
    return *this == other;
  }
  string Label::toString()
  {
    return this->labelname;
  }

  // Number Item
  Number::Number(int64_t n)
  {
    this->num = n;
  }
  int64_t Number::get()
  {
    return this->num;
  }
  bool Number::operator==(const Number &other)
  {
    return *this == other;
  }
  string Number::toString()
  {
    return to_string(this->num);
  }

  // Variable Item
  Variable::Variable(string v)
  {
    this->variableName = v;
  }
  string Variable::get()
  {
    return this->variableName;
  }
  bool Variable::operator==(const Variable &other)
  {
    return *this == other;
  }
  string Variable::toString()
  {
    return this->variableName;
  }
  // Operation Item
  Operation::Operation(string op)
  {
    this->op = op;
  }
  string Operation::get()
  {
    return this->op;
  }
  string Operation::toString()
  {
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


//assignment 
void Instruction_assignment::accept(Visitor* v) {
  v->visit(this);
}

void Instruction_load::accept(Visitor* v) {
  v->visit(this);
}

void Instruction_store::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_ret_not::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_ret_t::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_math::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_compare::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_br_label::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_br_t::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_call_noassign::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_call_assignment::accept(Visitor* v) {
  v->visit(this);
}
void Instruction_label::accept(Visitor* v) {
  v->visit(this);
}
  }