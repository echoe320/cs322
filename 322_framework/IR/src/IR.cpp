#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// #include <liveness.h>
#include <IR.h>

// included libraries
#include <unordered_set>
#include <unordered_map>

// using namespace std;
bool doiprint = false;

namespace IR {
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

  // Array
  Array::Array(std::string name, int dim) : Variable(int64_arr, name){
    this->dimension = dim;
  }

  // Tuple
  Tuple::Tuple(std::string name) : Variable(tup, name){
  }

  //* ============================= INSTRUCTIONS =============================

  // Instruction_def
  void Instruction_def::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_def::Instruction_def(Item *t, Item *v) {
    this->typ = t;
    this->var = v;
  }
  std::tuple<Item*, Item*> Instruction_def::get() {
    std::tuple<Item*, Item*> tup(this->typ, this->var);
    return tup;
  }
  std::string Instruction_def::typeAsString() {
    return "define instruction";
  }

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
    return "assignment instruction";
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

  // Instruction_length
  void Instruction_length::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_length::Instruction_length(Item *dest, Item *source, Item *d) {
    this->dst = dest;
    this->src = source;
    this->dim = d;
  }
  std::tuple<Item*, Item*, Item*> Instruction_length::get() {
    std::tuple<Item*, Item*, Item*> tup(this->dst, this->src, this->dim);
    return tup;
  }
  std::string Instruction_length::typeAsString() {
    return "length instruction";
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

  // Instruction_array
  void Instruction_array::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_array::Instruction_array(Item *dest, std::vector<Item*> as) {
    this->dst = dest;
    this->args = as;
  }
  std::tuple<Item*, std::vector<Item*>> Instruction_array::get() {
    std::tuple<Item*, std::vector<Item*>> tup(this->dst, this->args);
    return tup;
  }
  std::string Instruction_array::typeAsString() {
    return "array instruction";
  }

  // Instruction_tuple
  void Instruction_tuple::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_tuple::Instruction_tuple(Item *dest, std::vector<Item*> as) {
    this->dst = dest;
    this->args = as;
  }
  std::tuple<Item*, std::vector<Item*>> Instruction_tuple::get() {
    std::tuple<Item*, std::vector<Item*>> tup(this->dst, this->args);
    return tup;
  }
  std::string Instruction_tuple::typeAsString() {
    return "tuple instruction";
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

  // te_br_label
  void te_br_label::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  te_br_label::te_br_label(Item *target) {
    this->label = target;
  }
  std::tuple<Item*> te_br_label::get(){
    return std::make_tuple(this->label);
  }
  std::string te_br_label::typeAsString() {
    return "te_br_label instruction";
  }

  // te_br_t
  void te_br_t::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  te_br_t::te_br_t(Item *tar, Item *l1, Item *l2) {
    this->t = tar;
    this->label1 = l1;
    this->label2 = l2;
  }
  std::tuple<Item*, Item*, Item*> te_br_t::get(){
    return std::make_tuple(this->t, this->label1, this->label2);
  }
  std::string te_br_t::typeAsString() {
    return "te_br_t instruction";
  }

  // te_return
  void te_return::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  te_return::te_return(){} // empty constructor? doesn't throw errors now
  std::string te_return::typeAsString() {
    return "te_return instruction";
  }

  // te_return_t
  void te_return_t::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  te_return_t::te_return_t(Item *tar) {
    this->t = tar;
  }
  std::tuple<Item*> te_return_t::get(){
    return std::make_tuple(this->t);
  }
  std::string te_return_t::typeAsString() {
    return "te_return_t instruction";
  }

}