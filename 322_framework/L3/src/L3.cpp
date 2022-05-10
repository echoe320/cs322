#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// #include <liveness.h>
#include <L3.h>

// included libraries
#include <unordered_set>
#include <unordered_map>

// using namespace std;
bool doiprint = false;

namespace L3 {
  /*
   * Constructor/Member Functions
   */
  
  //* ============================= ITEMS =============================
  //Number
  Number::Number(int64_t n){
    this->num = n;
  }
  int64_t Number::get(void){
    return this->num;
  }
  std::string Number::toString(void) {
    return std::to_string(this->num);
  }

  //Label
  Label::Label(std::string ln){
    this->labelName = ln;
  }
  std::string Label::get(void){
    return this->labelName;
  }
  std::string Label::toString(void) {
    return this->labelName;
  }

  //Variable
  Variable::Variable(var_type tp, std::string vn){
    this->varType = tp;
    this->varName = vn;
  }
  std::pair<var_type, std::string> Variable::get(void){
    std::pair<var_type, std::string> var{this->varType, this->varName};
    return var;
  }
  std::string Variable::toString(void) {
    return this->varName;
  }

  //Operation
  Operation::Operation(opCode on){
    this->opName = on;
  }
  opCode Operation::get(void){
    return this->opName;
  }
  std::string Operation::toString(void) {
    return "";
  }

  // // Array
  // Array::Array(std::string name, int dim) : Variable(int64_arr, name){
  //   this->dimension = dim;
  // }

  // // Tuple
  // Tuple::Tuple(std::string name) : Variable(tup, name){
  // }

  //Callee
  Callee::Callee(calleeCode cc){
    this->ce = cc;
  }
  calleeCode Callee::get(void){
    return this->ce;
  }
  std::string Callee::toString(void) {
    return "callee";
  }

  //* ============================= INSTRUCTIONS =============================

  // Instruction_assignment
  void Instruction_assignment::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_assignment::Instruction_assignment(Item *dest, Item *source) {
    this->dst = dest;
    this->src = source;
  }
  std::tuple<Item*, Item*> Instruction_assignment::get() {
    std::tuple<Item*, Item*> tup(this->dst, this->src);
    return tup;
  }
  std::string Instruction_assignment::typeAsString() {
    return "Instruction_assignment";
  }

  // Instruction_op
  void Instruction_op::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_op::Instruction_op(Item *dest, Item *a1, Item *a2, Item *ope) {
    this->dst = dest;
    this->arg1 = a1;
    this->arg2 = a2;
    this->op = ope;
  }
  std::tuple<Item*, Item*, Item*, Item*> Instruction_op::get() {
    std::tuple<Item*, Item*, Item*, Item*> tup(this->dst, this->arg1, this->arg2, this->op);
    return tup;
  }
  std::string Instruction_op::typeAsString() {
    return "op instruction";
  }

  // Instruction_load
  void Instruction_load::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_load::Instruction_load(Item *dest, Item *source) {
    this->dst = dest;
    this->src = source;
  }
  std::tuple<Item*, Item*> Instruction_load::get() {
    std::tuple<Item*, Item*> tup(this->dst, this->src);
    return tup;
  }
  std::string Instruction_load::typeAsString() {
    return "load instruction";
  }

  // Instruction_store
  void Instruction_store::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_store::Instruction_store(Item *dest, Item *source) {
    this->dst = dest;
    this->src = source;
  }
  std::tuple<Item*, Item*> Instruction_store::get() {
    std::tuple<Item*, Item*> tup(this->dst, this->src);
    return tup;
  }
  std::string Instruction_store::typeAsString() {
    return "store instruction";
  }

  // Instruction_call
  void Instruction_call::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_call::Instruction_call(Item *call, std::vector<Item*> as) {
    this->callee = call;
    this->args = as;
  }
  std::tuple<Item*, std::vector<Item*>> Instruction_call::get() {
    std::tuple<Item*, std::vector<Item*>> tup(this->callee, this->args);
    return tup;
  }
  std::string Instruction_call::typeAsString() {
    return "call instruction";
  }

  // Instruction_call_assign
  void Instruction_call_assign::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_call_assign::Instruction_call_assign(Item *dest, Item *call, std::vector<Item*> as) {
    this->dst = dest;
    this->callee = call;
    this->args = as;
  }
  std::tuple<Item*, Item*, std::vector<Item*>> Instruction_call_assign::get() {
    std::tuple<Item*, Item*, std::vector<Item*>> tup(this->dst, this->callee, this->args);
    return tup;
  }
  std::string Instruction_call_assign::typeAsString() {
    return "call assign instruction";
  }

  // Instruction_label
  void Instruction_label::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_label::Instruction_label(Item *target) {
    this->label = target;
  }
  std::tuple<Item*> Instruction_label::get(){
    return std::make_tuple(this->label);
  }
  std::string Instruction_label::typeAsString() {
    return "label instruction";
  }

  // Instruction_br_label
  void Instruction_br_label::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_br_label::Instruction_br_label(Item *target) {
    this->label = target;
  }
  std::tuple<Item*> Instruction_br_label::get(){
    return std::make_tuple(this->label);
  }
  std::string Instruction_br_label::typeAsString() {
    return "Instruction_br_label instruction";
  }

  // Instruction_br_t
  void Instruction_br_t::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_br_t::Instruction_br_t(Item *tar, Item *l1) {
    this->t = tar;
    this->label1 = l1;
  }
  std::tuple<Item*, Item*, Item*> Instruction_br_t::get(){
    return std::make_tuple(this->t, this->label1, this->label2);
  }
  std::string Instruction_br_t::typeAsString() {
    return "Instruction_br_t instruction";
  }

  // Instruction_return
  void Instruction_return::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_return::Instruction_return(){} // empty constructor? doesn't throw errors now
  std::string Instruction_return::typeAsString() {
    return "Instruction_return instruction";
  }

  // Instruction_return_t
  void Instruction_return_t::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_return_t::Instruction_return_t(Item *tar) {
    this->t = tar;
  }
  std::tuple<Item*> Instruction_return_t::get(){
    return std::make_tuple(this->t);
  }
  std::string Instruction_return_t::typeAsString() {
    return "Instruction_return_t instruction";
  }

  std::string get_enum_string (int enum_value) {
    return (op_enum_str[enum_value]);
  }

}