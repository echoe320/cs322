#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// #include <liveness.h>
#include <L2.h>

// using namespace std;
bool doiprint = false;

namespace L2 {
  /*
   * Constructor/Member Functions
   */
  
  //* ============================= ITEMS =============================
  //Register
  Register::Register(reg regi) {
    this->r = regi;
    // std::cout << "set register " << get_enum_string(regi) << "\n";
  }
  reg Register::get(void) {
    return this->r;
  }
  std::string Register::toString(void) {
    // use enum as an index to access the string version of register
    // std::cout << "toString\n";
    return get_enum_string(this->r);
  }

  //Memory
  Memory::Memory(Item *arg, Item *os){
    this->rv = arg;
    this->offset = os;
  }
  std::pair<Item *, Item *> Memory::get(void){
    std::pair<Item *, Item *> res{this->rv, this->offset};
    return res; // res.first
  }
  std::string Memory::toString(void) {
    // auto reg_temp = dynamic_cast<Register *>(this->rv);
    auto num_temp = dynamic_cast<Number *>(this->offset);
    return "mem " + this->rv->toString() + " " + num_temp->toString();
  }

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
  Variable::Variable(std::string vn){
    this->varName = vn;
  }
  std::string Variable::get(void){
    return this->varName;
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
    return get_op_string(this->opName);
  }

  //Runtime
  Runtime::Runtime(runtimeCode rt) {
    this->runtime = rt;
  }
  runtimeCode Runtime::get(void) {
    return this->runtime;
  }
  std::string Runtime::toString(void) {
    return get_rt_string(this->runtime);
  }


  //* ============================= INSTRUCTIONS =============================
  // RETURN INSTRUCTION
  void Instruction_ret::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  Instruction_ret::Instruction_ret(){} // empty constructor? doesn't throw errors now
  std::string Instruction_ret::typeAsString() {
    return "return instruction";
  }
  std::string Instruction_ret::toString() {
    return "return";
  }

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
    // return std::make_tuple(this->src, this->dst);
    std::tuple<Item*, Item*> tup(this->src, this->dst);
    return tup;
  }
  std::string Instruction_assignment::typeAsString() {
    return "assignment instruction";
  }
  std::string Instruction_assignment::toString() {
    return this->dst->toString() + " <- " + this->src->toString();
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
  std::string Instruction_arithmetic::typeAsString() {
    return "arithmetic instruction";
  }
  std::string Instruction_arithmetic::toString() {
    return this->dst->toString() + " " + this->op->toString() + " " + this->src->toString();
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
  std::string Instruction_crement::typeAsString() {
    return "crement instruction";
  }
  std::string Instruction_crement::toString() {
    return this->dst->toString() + this->op->toString();
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
  std::string Instruction_shift::typeAsString() {
    return "shift instruction";
  }
  std::string Instruction_shift::toString() {
    return this->dst->toString() + " " + this->op->toString() + " " + this->src->toString();
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
  std::string Instruction_cmp::typeAsString() {
    return "compare instruction";
  }
  std::string Instruction_cmp::toString() {
    return this->dst->toString() + " <- " + this->arg1->toString() + " " + this->op->toString() + " " + this->arg2->toString();
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
  std::string Instruction_cjump::typeAsString() {
    return "conditional jump instruction";
  }
  std::string Instruction_cjump::toString() {
    return "cjump " + this->arg1->toString() + " " + this->op->toString() + " " + this->arg2->toString() + " " + this->label->toString();
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
  std::string Instruction_lea::typeAsString() {
    return "lea instruction";
  }
  std::string Instruction_lea::toString() {
    return this->dst->toString() + " @ " + this->arg1->toString() + " " + this->arg2->toString() + " " + this->mult->toString();
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
  std::string Instruction_calls::typeAsString() {
    return "calls instruction";
  }
  std::string Instruction_calls::toString() {
    return "call " + this->u->toString() + " " + this->N->toString();
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
  std::string Instruction_runtime::typeAsString() {
    return "runtime call instruction";
  }
  std::string Instruction_runtime::toString() {
    auto rt_temp = dynamic_cast<Runtime*>(this->runtime);
    auto num_temp = dynamic_cast<Number*>(this->N); 
    return "call " + rt_temp->toString() + " " + num_temp->toString();
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
  std::string Instruction_label::typeAsString() {
    return "label instruction";
  }
  std::string Instruction_label::toString() {
    return this->label->toString();
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
  std::string Instruction_goto::typeAsString() {
    return "goto instruction";
  }
  std::string Instruction_goto::toString() {
    return "goto " + this->label->toString();
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
  std::string Instruction_stackarg::typeAsString() {
    return "stackarg instruction";
  }
  std::string Instruction_stackarg::toString() {
    return this->dst->toString() + " <- " + this->M->toString();
  }

  std::string get_enum_string (int enum_value) {
    return (reg_enum_str[enum_value]);
  }

  std::string get_op_string (int enum_value) {
    return (op_enum_str[enum_value]);
  }

  std::string get_rt_string (int enum_value) {
    return (rt_enum_str[enum_value]);
  }

  std::string get_color (int idx) {
    return all_color_list[idx];
  }


  //* FUNCTION - setting successors and predecessors
  void Function::findSuccessorsPredecessors() {
    int instructions_len = this->instructions.size();

    //* initialize label target map
    std::unordered_map<std::string, int> label_dict;
    for (int i = 0; i < instructions_len; i++) {
      if (dynamic_cast<Instruction_label *>(this->instructions[i]) != nullptr) {
        auto inst_temp = static_cast<Instruction_label *>(this->instructions[i]);
        auto label = std::get<0>(inst_temp->get());
        label_dict[label->toString()] = i;
      }
    }

    for (int ii = 0; ii < instructions_len; ii++) {
      // return instruction has no successors
      if (dynamic_cast<Instruction_ret *>(this->instructions[ii]) != nullptr) {
        if (doiprint) std::cout << "return succ" << std::endl;
        this->instructions[ii]->successor_idx.clear();
      }

      // call tensor_error has no successors
      else if (dynamic_cast<Instruction_runtime *>(this->instructions[ii]) != nullptr) {
        if (doiprint) std::cout << "runtime succ" << std::endl;
        auto inst_temp = static_cast<Instruction_runtime *>(this->instructions[ii]);
        auto rt = static_cast<Runtime *>(std::get<0>(inst_temp->get()));
        auto rt_code = rt->get();
        if (rt_code == rt_tensor_error) continue;
        else this->instructions[ii]->successor_idx.insert(ii + 1);
      }

      //cjump has two successors: target + instruction right after
      else if (dynamic_cast<Instruction_cjump *>(this->instructions[ii]) != nullptr) {
        if (doiprint) std::cout << "cjump succ" << std::endl;
        this->instructions[ii]->successor_idx.insert(ii + 1);
        auto inst_temp = dynamic_cast<Instruction_cjump *>(this->instructions[ii]);
        auto target = static_cast<Label *>(std::get<2>(inst_temp->get()));
        this->instructions[ii]->successor_idx.insert(label_dict[target->toString()]);
      }

      // goto instruction's successor is the target (label)
      else if (dynamic_cast<Instruction_goto *>(this->instructions[ii]) != nullptr) {
        if (doiprint) std::cout << "goto succ" << std::endl;
        auto inst_temp = dynamic_cast<Instruction_goto *>(this->instructions[ii]);
        auto target = static_cast<Label *>(std::get<0>(inst_temp->get()));
        this->instructions[ii]->successor_idx.insert(label_dict[target->toString()]);
      }

      // if not a special case, successor is the instruction right after
      else {
        if (doiprint) std::cout << "everyday succ" << std::endl;
        this->instructions[ii]->successor_idx.insert(ii + 1);
      }
    }
  }

  void Function::printINOUTsets() {
      std::cout << "(" << std::endl;
      std::cout << "(in " << std::endl;
      int count = 0;
      for (auto in : this->IN) {
        std::vector<std::string> in_sorted;
        std::cout << "(";
        for (auto it = in.begin(); it != in.end(); ++it) {
          auto temp = *it;
          in_sorted.push_back(temp->toString());
        }
        count++;
        std::sort(in_sorted.begin(), in_sorted.end());
        for (std::string i : in_sorted)
          if (i == in_sorted.back()) {
            std::cout << i;
          } else {
            std::cout << i << ' ';
          }
        std::cout << ")" << std::endl;
      }
      std::cout << ")\n" << "\n";
      std::cout << "(out " << std::endl;
      count = 0;
      for (auto out : this->OUT) {
        std::vector<std::string> out_sorted;
        std::cout << "(";
        for (auto it = out.begin(); it != out.end(); ++it) {
          auto temp = *it;
          out_sorted.push_back(temp->toString());
        }
        count++;
        std::sort(out_sorted.begin(), out_sorted.end());
        for (std::string i: out_sorted)
          if (i == out_sorted.back()) {
            std::cout << i;
          } else {
            std::cout << i << ' ';
          }
        std::cout << ")" << std::endl;
      }
      std::cout << ")" << std::endl;
      std::cout << "\n" << ")\n" << "\n";
  }

  // void Function::printInterferenceGraph() {
  //   for (auto it = this->interference_graph.begin(); it != this->interference_graph.end(); ++it) {
  //     std::cout << it->first << " ";
  //     for (auto item : it->second) {
  //       std::cout << item << " ";
  //     }
  //     std::cout << std::endl;
  //   }
  // }
}