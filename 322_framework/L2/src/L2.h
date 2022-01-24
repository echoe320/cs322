#pragma once

#include <vector>
#include <string>

namespace L2 {
  class visitor;

  enum Register {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  enum Operation {op_add, op_minus, op_multiply, op_divide, op_lshift, op_rshift};
  
  enum iType {ret, assignment, arithmetic, crement, shift, cmp, cjump, lea, calls, _label, gotoo, stackarg};

  class Item {
    public:
      std::string labelName;
      //Register r;
      std::string Register;
      std::string offset; // doubles as number

      bool isARegister = false;
      bool isMem = false;
      bool isNum = false;
      bool isLabel = false;
      bool isVar = false;
      //int offset;
  };

  //class 

  class operation {
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
}