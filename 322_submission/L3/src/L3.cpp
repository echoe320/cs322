#include "L3.h"
#include <iostream>
#include <string>

using namespace std;

namespace L3
{
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

  void Function::format_function() {
    // ::cout << "(" << this->name << endl;
    // ::cout << "\t" << this->arguments << endl;
    // for (auto i : this->instructions) {
    //   ::cout << "\t" << i->toString() << endl;
    // }

    // ::cout << ")" << endl;
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


//   Instruction_ret * Instruction_ret::clone() { return new Instruction_ret(*this); }
//   Instruction_assignment * Instruction_assignment::clone() { return new Instruction_assignment(*this); }
//   Instruction_load * Instruction_load::clone() { return new Instruction_load(*this); }
//   Instruction_shift * Instruction_shift::clone() { return new Instruction_shift(*this); }
//   Instruction_store * Instruction_store::clone() { return new Instruction_store(*this); }
//   Instruction_stack * Instruction_stack::clone() { return new Instruction_stack(*this); }
//   Instruction_aop * Instruction_aop::clone() { return new Instruction_aop(*this); }
//   Instruction_store_aop * Instruction_store_aop::clone() { return new Instruction_store_aop(*this); }
//   Instruction_load_aop * Instruction_load_aop::clone() { return new Instruction_load_aop(*this); }
//   Instruction_compare * Instruction_compare::clone() { return new Instruction_compare(*this); }
//   Instruction_cjump * Instruction_cjump::clone() { return new Instruction_cjump(*this); }
//   Instruction_call * Instruction_call::clone() { return new Instruction_call(*this); }
//   Instruction_call_print * Instruction_call_print::clone() { return new Instruction_call_print(*this); }
//   Instruction_call_input * Instruction_call_input::clone() { return new Instruction_call_input(*this); }
//   Instruction_call_allocate * Instruction_call_allocate::clone() { return new Instruction_call_allocate(*this); }
//   Instruction_call_error * Instruction_call_error::clone() { return new Instruction_call_error(*this); }
//   Instruction_label * Instruction_label::clone() { return new Instruction_label(*this); }
//   Instruction_increment * Instruction_increment::clone() { return new Instruction_increment(*this); }
//   Instruction_decrement * Instruction_decrement::clone() { return new Instruction_decrement(*this); }
//   Instruction_at * Instruction_at::clone() { return new Instruction_at(*this); }
//   Instruction_goto * Instruction_goto::clone() { return new Instruction_goto(*this); }


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