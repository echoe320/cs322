#pragma once

#include <vector>
#include <string>

#include <unordered_set>

namespace L2 {
  class visitor;

  enum reg {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  enum Operation {op_add, op_minus, op_multiply, op_divide, op_lshift, op_rshift};
  
  enum iType {ret, assignment, arithmetic, crement, shift, cmp, cjump, lea, calls, runtime, _label, gotoo, stackarg};

  std::vector<std::string> arg_registers[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax"};

  class Item {
    public:
      //virtual void print_obj = 0;
      std::string labelName;
      reg r;
      //std::string Register;
      std::string offset; // doubles as number

      bool isARegister = false;
      bool isMem = false;
      bool isNum = false;
      bool isLabel = false;
      bool isVar = false;
      //int offset;
  };

  class Register : public Item {
    public:
      
    private:
      reg r;
  };

  class Memory : public Item {

  };

  class Number : public Item {

  };

  class Label : public Item {

  };

  class Variable : public Item {

  };

  class operation : public Item {
    public:
      std::string op;
  };

  /*
   * Instruction interface.
   */
  class Instruction{
    public:
      iType id;
  };

  /*
   * Instructions.
   */
  class Instruction_ret : public Instruction{
  };

  class Instruction_assignment : public Instruction{
    public:
      Item src, dst;
  };

  class Instruction_arithmetic : public Instruction{
    public:
      Item src, dst;
      operation op;
  };

  class Instruction_crement : public Instruction{
    public:
      Item dst;
      operation op;
  };

  class Instruction_shift : public Instruction{
    public:
      Item src, dst;
      operation op;
  };

  class Instruction_cmp : public Instruction{
    public:
      Item dst, arg1, arg2;
      operation op;
  };

  class Instruction_cjump : public Instruction{
    public:
      Item arg1, arg2, label;
      operation op;
  };

  class Instruction_lea : public Instruction{
    public:
      Item dst, arg1, arg2, multiple;
  };

  class Instruction_calls : public Instruction{
    public:
      Item u, N; //call u N
      // bool isRuntime = false;
  };

  class Instruction_runtime : public Instruction{
    public:
      Item runtime, N;
  };

  class Instruction_label : public Instruction{
    public:
      Item label;
  };

  class Instruction_goto : public Instruction{
    public:
      Item label;
  };

  class Instruction_stackarg : public Instruction {
    public:
      Item dst, M;
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      //int64_t locals; //! FUNCTION DOES NOT HAVE LOCALS IN L2
      std::vector<Instruction *> instructions;
  };

  /*
   * Program.
   */
  class Program{
    public:
      std::string entryPointLabel;
      std::vector<Function *> functions;
  };

  //* LIVENESS SETS


  //Callee save register items
  Item reg_12;
  reg_12->r = r12;

  Item reg_13;
  reg_13->r = r13;

  Item reg_14;
  reg_14->r = r14;

  Item reg_15;
  reg_15->r = r15;

  Item reg_bp;
  reg_bp->r = rbp;

  Item reg_bx;
  reg_bx->r = rbx;

  //Callee save register unordered-set
  std::unordered_set<Item> callee_save_set[6];
  callee_save_set.insert(reg_12);
  callee_save_set.insert(reg_13);
  callee_save_set.insert(reg_14);
  callee_save_set.insert(reg_15);
  callee_save_set.insert(reg_bp);
  callee_save_set.insert(reg_bx);

  //Caller save register items
  Item reg_10;
  reg_10->r = r10;

  Item reg_11;
  reg_11->r = r11;

  Item reg_8;
  reg_8->r = r8;

  Item reg_9;
  reg_9->r = r9;

  Item reg_ax;
  reg_ax->r = rax;

  Item reg_cx;
  reg_cx->r = rcx;
  
  Item reg_di;
  reg_di->r = rdi;

  Item reg_dx;
  reg_dx->r = rdx;

  Item reg_si;
  reg_si->r = rsi;
  
  
  //Caller save register unordered-set
  std::unordered_set<Item> caller_save_set[9];
  caller_save_set.insert(reg_10);
  caller_save_set.insert(reg_11);
  caller_save_set.insert(reg_8);
  caller_save_set.insert(reg_9);
  caller_save_set.insert(reg_ax);
  caller_save_set.insert(reg_cx);
  caller_save_set.insert(reg_di);
  caller_save_set.insert(reg_dx);
  caller_save_set.insert(reg_si);

  // arg register sets
  // std::vector<std::unordered_set<Item>> arg_registers[7];
  // arg_registers[0] = {};
  // arg_registers[1] = {reg_di};
  // arg_registers[2] = {reg_di, reg_si};
  // arg_registers[3] = {reg_di, reg_si, reg_dx};
  // arg_registers[4] = {reg_di, reg_si, reg_dx, reg_cx};
  // arg_registers[5] = {reg_di, reg_si, reg_dx, reg_cx, reg_8};
  // arg_registers[6] = {reg_di, reg_si, reg_dx, reg_cx, reg_8, reg_9};
}