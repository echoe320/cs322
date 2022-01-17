#pragma once

#include <vector>
#include <string>

namespace L1 {

  //enum Register {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  class Item {
    public:
      std::string labelName;
      //Register r;
      std::string Register;
      std::string offset; // doubles as number

      bool isARegister;
      //allow mem to be an item
      bool isMem;
      bool isConst;
      //int offset;
  };

  class operation {
    public:
      std::string op;
  };

  // class mem {
  //   public:
  //     Register r;
  //     int num;
  // };

  // //registers
  // class w {};
  
  // //argRegisters
  // class a : w {};

  // //rcx
  // class sx {};

  // //register or N or label
  // class st {};

  // //x or N
  // class t : s {};

  // //register or label AKA ITEM CLASS
  // class u {};

  // //ALL registers (including rsp)
  // class x {};

  /*
   * Instruction interface.
   */
  class Instruction{
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

  //enum Operation {ADD, MINUS, MULT, AND, LSHIFT, RSHIFT};

  class Instruction_arithmetic : public Instruction{
    public:
      Item src, dst;
      operation op;
      //Operation a;
  };

  class Instruction_shift : public Instruction{
    public:
      Item src, dst;
      operation op;
      //Operation a;
  };

  //enum Comparison {LESS, LESSEQ, EQUAL};

  class Instruction_cmp : public Instruction{
    public:
      Item dst, arg1, arg2;
      operation op;
      //Comparison cmp;
  };

  class Instruction_cjump : public Instruction{
    public:
      Item arg1, arg2, label;
      operation op;
      //Comparison cmp;
  };

  class Instruction_lea : public Instruction{
    public:
      Item src, dst, label;
      operation op;
  };

  class Instruction_calls : public Instruction{
    public:
      Item u, N; //call u N
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      int64_t locals;
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
