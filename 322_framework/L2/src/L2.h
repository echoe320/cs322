#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>

namespace L2 {
  // class visitor;

  enum reg {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  static const char* reg_enum_str[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsp"};

  enum opCode {op_add, op_minus, op_multiply, op_AND, op_lshift, op_rshift, op_inc, op_dec, cmp_equals, cmp_less, cmp_lesseq};
  
  static const char* op_enum_str[] = {"+=", "-=", "*=", "&=", "<<=", ">>=", "++", "--", "=", "<", "<="};

  enum runtimeCode {rt_print, rt_input, rt_allocate, rt_tensor_error};

  static const char* rt_enum_str[] = {"print", "input", "allocate", "tensor-error"};

  // enum all_color_list{red, orange, yellow, green, blue, indigo, purple, pink, brown, black, white, gray, maroon, navy, periwinkle};

  static const char* all_color_list[] = {"red", "orange", "yellow", "green", "blue", "indigo", "purple", "pink", "brown", "black", "white", "gray", "maroon", "navy", "periwinkle"};

  //Item Class + subclasses
  class Item {
    public:
      virtual std::string toString(void) = 0;
      bool isSpill;
  };

  class Register : public Item {
    public:
      Register(reg regi);
      reg get(void);
      std::string toString(void) override;
    private:
      reg r;
  };

  class Memory : public Item {
    public:
      Memory(Item *arg, Item *os);
      std::pair<Item *, Item *> get(void);
      std::string toString(void) override;
    private:
      Item *rv, *offset;
  };

  class Number : public Item {
    public:
      Number(int64_t n);
      int64_t get(void);
      std::string toString(void) override;
    private:
      int64_t num;
  };

  class Label : public Item {
    public:
      Label(std::string ln);
      std::string get(void);
      std::string toString(void) override;
    private:
      std::string labelName;
  };

  class Variable : public Item {
    public:
      Variable(std::string vn);
      std::string get(void);
      std::string toString(void) override;
      bool isSpill = false;
    private:
      std::string varName;
  };

  class Operation : public Item {
    public:
      Operation(opCode on);
      opCode get(void);
      std::string toString(void) override;
    private:
      opCode opName;
  };

  class Runtime : public Item {
    public:
      Runtime(runtimeCode rt);
      runtimeCode get(void);
      std::string toString(void) override;
    private:
      runtimeCode runtime;
  };

  // Declare Visitor first
  class Visitor;

  /*
   * Instructions.
   */
  class Instruction {
    public:
      virtual void Accept(Visitor *visitor) = 0;
      virtual std::string typeAsString(void) = 0;
      virtual std::string toString(void) = 0;

      std::set<int> predecessor_idx;
      std::set<int> successor_idx;
      std::unordered_set<Item *> reads; // Gen
      std::unordered_set<Item *> writes; // Kill
      std::unordered_set<Item *> IN; // IN
      std::unordered_set<Item *> OUT; // OUT
  };

  class Instruction_ret : public Instruction{
    public:
      Instruction_ret();
      void Accept(Visitor *visitor) override;
      std::string typeAsString(void) override;
      std::string toString(void) override;
  };

  class Instruction_assignment : public Instruction{
    public:
      Instruction_assignment(Item *source, Item *dest);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get(); //access individual fields by std::get<idx>(element.get())
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *src, *dst; // src = std::get<0>(element.get());
  };

  class Instruction_arithmetic : public Instruction{
    public:
      Instruction_arithmetic(Item *source, Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *src, *dst, *op;
  };

  class Instruction_crement : public Instruction{
    public:
      Instruction_crement(Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *dst, *op;
  };

  class Instruction_shift : public Instruction{
    public:
      Instruction_shift(Item *source, Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *src, *dst, *op;
  };

  class Instruction_cmp : public Instruction{
    public:
      Instruction_cmp(Item *dest, Item *one, Item *two, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *dst, *arg1, *arg2, *op;
  };

  class Instruction_cjump : public Instruction{
    public:
      Instruction_cjump(Item *one, Item *two, Item *target, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *arg1, *arg2, *label, *op;
  };

  class Instruction_lea : public Instruction{
    public:
      Instruction_lea(Item *dest, Item *one, Item *two, Item *multiple);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *dst, *arg1, *arg2, *mult;
  };

  class Instruction_calls : public Instruction{
    public:
      Instruction_calls(Item *target, Item *numArgs);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *u, *N; //call u N
  };

  class Instruction_runtime : public Instruction{
    public:
      Instruction_runtime(Item *target, Item *numArgs);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *runtime, *N;
  };

  class Instruction_label : public Instruction{
    public:
      Instruction_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *label;
  };

  class Instruction_goto : public Instruction{
    public:
      Instruction_goto(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *label;
  };

  class Instruction_stackarg : public Instruction {
    public:
      Instruction_stackarg(Item *dest, Item *offset);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      std::string toString(void) override;
    private:
      Item *dst, *M;
  };

  /*
   * Instruction interface.
   */
  class Visitor {
    public:
      virtual void VisitInstruction(Instruction_ret *element) = 0;
      virtual void VisitInstruction(Instruction_assignment *element) = 0;
      virtual void VisitInstruction(Instruction_arithmetic *element) = 0;
      virtual void VisitInstruction(Instruction_crement *element) = 0;
      virtual void VisitInstruction(Instruction_shift *element) = 0;
      virtual void VisitInstruction(Instruction_cmp *element) = 0;
      virtual void VisitInstruction(Instruction_cjump *element) = 0;
      virtual void VisitInstruction(Instruction_lea *element) = 0;
      virtual void VisitInstruction(Instruction_calls *element) = 0;
      virtual void VisitInstruction(Instruction_runtime *element) = 0;
      virtual void VisitInstruction(Instruction_label *element) = 0;
      virtual void VisitInstruction(Instruction_goto *element) = 0;
      virtual void VisitInstruction(Instruction_stackarg *element) = 0;
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      int64_t num_locals;
      std::vector<Instruction *> instructions;
      void findSuccessorsPredecessors();
      //Initialize vectors
      std::vector<std::unordered_set<Item *>> GEN; //* GEN[i] = all variables read by instruction i
      std::vector<std::unordered_set<Item *>> KILL;//* KILL[i] = all variables written/defined by instruction i
      std::vector<std::unordered_set<Item *>> IN;
      std::vector<std::unordered_set<Item *>> OUT;
      void printINOUTsets();

      std::set<Variable*> func_vars;
      bool allVarsSpilled = false;
      int spill_count = 0;
  };

  /*
   * Program.
   */
  class Program{
    public:
      std::string entryPointLabel;
      std::vector<Function *> functions;
      std::vector<Variable*> toSpill;
      std::string prefix;
  };

  std::string get_enum_string (int enum_value);

  std::string get_op_string (int enum_value);

  std::string get_rt_string (int enum_value);

  std::string get_color (int idx);
}