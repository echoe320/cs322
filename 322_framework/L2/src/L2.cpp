#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// #include <liveness.h>
#include <L2.h>

// included libraries
#include <unordered_set>

using namespace std;

namespace L2 {
  /*
   * Constructor/Member Functions
   */
  
  //* ============================= ITEMS =============================
  //Register
  Register::Register(reg regi) {
    this->r = regi;
  }
  reg Register::get(void) {
    return this->r;
  }
  std::string Register::toString(void) {
    return "";
  }

  //Memory
  Memory::Memory(reg regi, int64_t os){
    this->r = regi;
    this->offset = os;
  }
  std::pair<L2::reg, int64_t> Memory::get(void){
    std::pair<L2::reg, int64_t> res{this->r, this->offset};
    return res; // res.first
  }
  std::string Memory::toString(void) {
    return "";
  }

  //Number
  Number::Number(int64_t n){
    this->num = n;
  }
  int64_t Number::get(void){
    return this->num;
  }
  std::string Number::toString(void) {
    return "";
  }

  //Label
  Label::Label(std::string ln){
    this->labelName = ln;
  }
  std::string Label::get(void){
    return this->labelName;
  }
  std::string Label::toString(void) {
    return "";
  }

  //Variable
  Variable::Variable(std::string vn){
    this->varName = vn;
  }
  std::string Variable::get(void){
    return this->varName;
  }
  std::string Variable::toString(void) {
    return "";
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

  //Runtime
  Runtime::Runtime(runtimeCode rt) {
    this->runtime = rt;
  }
  runtimeCode Runtime::get(void) {
    return this->runtime;
  }
  std::string Runtime::toString(void) {
    return "";
  }


  //* ============================= INSTRUCTIONS =============================
  // RETURN INSTRUCTION
  void Instruction_ret::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_ret::Instruction_ret(){} // empty constructor? doesn't throw errors now   

  // ASSIGNMENT INSTRUCTION
  void Instruction_assignment::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_assignment::Instruction_assignment(Item *source, Item *dest) {
    this->src = source;
    this->dst = dest;
  }
  std::tuple<Item*, Item*> Instruction_assignment::get() {
    // std::pair<Item*, Item*> src_dst;
    // src_dst.first = this->src;
    // src_dst.second = this->dst;
    // return std::pair<Item*, Item*>{this->src, this->dst};
    return std::make_tuple(this->src, this->dst);
  }

  // ARITHMETIC INSTRUCTION
  void Instruction_arithmetic::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_arithmetic::Instruction_arithmetic(Item *source, Item *dest, Item *ope) {
    this->src = source;
    this->dst = dest;
    this->op = ope;
  }
  std::tuple<Item*, Item*, Item*> Instruction_arithmetic::get() {
    return std::make_tuple(this->src, this->dst, this->op);
  }

  // CREMENT INSTRUCTION
  void Instruction_crement::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_crement::Instruction_crement(Item *dest, Item *ope) {
    this->dst = dest;
    this->op = ope;
  }
  std::tuple<Item*, Item*> Instruction_crement::get() {
    return std::make_tuple(this->dst, this->op);
  }

  // SHIFT INSTRUCTION
  void Instruction_shift::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_shift::Instruction_shift(Item *source, Item *dest, Item *ope) {
    this->src = source;
    this->dst = dest;
    this->op = ope;
  }
  std::tuple<Item*, Item*, Item*> Instruction_shift::get() {
    return std::make_tuple(this->src, this->dst, this->op);
  }

  // COMPARE INSTRUCTION
  void Instruction_cmp::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_cmp::Instruction_cmp(Item *dest, Item *one, Item *two, Item *ope) {
    this->dst = dest;
    this->arg1 = one;
    this->arg2 = two;
    this->op = ope;
  }
  std::tuple<Item*, Item*, Item*, Item*> Instruction_cmp::get() {
    return std::make_tuple(this->dst, this->arg1, this->arg2, this->op);
  }

  // CONDITIONAL JUMP INSTRUCTION
  void Instruction_cjump::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_cjump::Instruction_cjump(Item *one, Item *two, Item *target, Item *ope) {
    this->arg1 = one;
    this->arg2 = two;
    this->label = target;
    this->op = ope;
  }
  std::tuple<Item*, Item*, Item*, Item*> Instruction_cjump::get() {
    return std::make_tuple(this->arg1, this->arg2, this->label, this->op);
  }

  // LEA INSTRUCTION
  void Instruction_lea::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_lea::Instruction_lea(Item *dest, Item *one, Item *two, Item *multiple) {
    this->dst = dest;
    this->arg1 = one;
    this->arg2 = two;
    this->mult = multiple;
  }
  std::tuple<Item*, Item*, Item*, Item*> Instruction_lea::get() {
    return std::make_tuple(this->dst, this->arg1, this->arg2, this->mult);
  }

  // CALL INSTRUCTION
  void Instruction_calls::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_calls::Instruction_calls(Item *target, Item *numArgs) {
    this->u = target;
    this->N = numArgs;
  }
  std::tuple<Item*, Item*> Instruction_calls::get() {
    return std::make_tuple(this->u, this->N);
  }

  // RUNTIME INSTRUCTION
  void Instruction_runtime::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_runtime::Instruction_runtime(Item *target, Item *numArgs) {
    this->runtime = target;
    this->N = numArgs;
  }
  std::tuple<Item*, Item*> Instruction_runtime::get() {
    return std::make_tuple(this->runtime, this->N);
  }

  // LABEL INSTRUCTION
  void Instruction_label::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_label::Instruction_label(Item *target) {
    this->label = target;
  }
  std::tuple<Item*> Instruction_label::get(){
    return std::make_tuple(this->label);
  }

  // GOTO INSTRUCTION
  void Instruction_goto::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_goto::Instruction_goto(Item *target) {
    this->label = target;
  }
  std::tuple<Item*> Instruction_goto::get(){
    return std::make_tuple(this->label);
  }

  // STACK-ARG INSTRUCTION
  void Instruction_stackarg::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_stackarg::Instruction_stackarg(Item *dest, Item *offset) {
    this->dst = dest;
    this->M = offset;
  }
  std::tuple<Item*, Item*> Instruction_stackarg::get(){
    return std::make_tuple(this->dst, this->M);
  }

  /*
   * defining enum to string
   */
  std::string get_enum_string (int enum_value) {
    return (reg_enum_str[enum_value]);
  }
}