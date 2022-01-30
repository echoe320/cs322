#pragma once

#include <vector>
#include <string>

#include <unordered_set>

namespace L2 {
  // class visitor;

  enum reg {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  static const char* reg_enum_str[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsp"};

  enum opCode {op_add, op_minus, op_multiply, op_AND, op_lshift, op_rshift, op_inc, op_dec, cmp_equals, cmp_less, cmp_lesseq};
  
  enum runtimeCode {rt_print, rt_input, rt_allocate, rt_tensor_error};

  //Item Class + subclasses
  class Item {
    public:
      virtual std::string toString(void) = 0;
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
      Memory(reg regi, int64_t os);
      std::pair<L2::reg, int64_t> get(void);
      std::string toString(void) override;
    private:
      reg r;
      int64_t offset;
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

  class Visitor;

  /*
   * Instructions.
   */
  class Instruction {
    public:
      virtual void Accept(Visitor *visitor) = 0;
      //virtual std::string toString(void) = 0;

      std::unordered_set<Instruction *> predecessors;
      std::unordered_set<Instruction *> successors;
      std::unordered_set<Item *> reads; // Gen
      std::unordered_set<Item *> writes; // Kill
  };

  class Instruction_ret : public Instruction{
    public:
      Instruction_ret();
      void Accept(Visitor *visitor) override;
  };

  class Instruction_assignment : public Instruction{
    public:
      Instruction_assignment(Item *source, Item *dest);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get(); //access individual fields by std::get<idx>(element.get())
    private:
      Item *src, *dst; // src = std::get<0>(element.get());
  };

  class Instruction_arithmetic : public Instruction{
    public:
      Instruction_arithmetic(Item *source, Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();

    private:
      Item *src, *dst, *op;
  };

  class Instruction_crement : public Instruction{
    public:
      Instruction_crement(Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
    private:
      Item *dst, *op;
  };

  class Instruction_shift : public Instruction{
    public:
      Instruction_shift(Item *source, Item *dest, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();
    private:
      Item *src, *dst, *op;
  };

  class Instruction_cmp : public Instruction{
    public:
      Instruction_cmp(Item *dest, Item *one, Item *two, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
    private:
      Item *dst, *arg1, *arg2, *op;
  };

  class Instruction_cjump : public Instruction{
    public:
      Instruction_cjump(Item *one, Item *two, Item *target, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
    private:
      Item *arg1, *arg2, *label, *op;
  };

  class Instruction_lea : public Instruction{
    public:
      Instruction_lea(Item *dest, Item *one, Item *two, Item *multiple);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
    private:
      Item *dst, *arg1, *arg2, *mult;
  };

  class Instruction_calls : public Instruction{
    public:
      Instruction_calls(Item *target, Item *numArgs);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
    private:
      Item *u, *N; //call u N
  };

  class Instruction_runtime : public Instruction{
    public:
      Instruction_runtime(Item *target, Item *numArgs);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
    private:
      Item *runtime, *N;
  };

  class Instruction_label : public Instruction{
    public:
      Instruction_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
    private:
      Item *label;
  };

  class Instruction_goto : public Instruction{
    public:
      Instruction_goto(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
    private:
      Item *label;
  };

  class Instruction_stackarg : public Instruction {
    public:
      Instruction_stackarg(Item *dest, Item *offset);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
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

  // class Visitor_Liveness : public Visitor {
  //   public:
  //     void VisitInstruction(const Instruction_assignment *element) const override;
      
  //     void VisitInstruction(const Instruction_calls *element) const override;
  // };

  //* don't need this yet
  // class Visitor_Interference : public Visitor {
  //   public:
  //     void VisitConcreteComponentA(const ConcreteComponentA *element) const override {
  //       std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
  //     }
  //     void VisitConcreteComponentB(const ConcreteComponentB *element) const override {
  //       std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
  //     }
  // };

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

  // arg register sets
  //std::vector<std::unordered_set<Item>> arg_registers(7);
  //arg_registers.push_back({reg_di, reg_si, reg_dx, reg_cx, reg_8, reg_9});
  //arg_registers.push_back();
  // arg_registers[1] = {reg_di};
  // arg_registers[2] = {reg_di, reg_si};
  // arg_registers[3] = {reg_di, reg_si, reg_dx};
  // arg_registers[4] = {reg_di, reg_si, reg_dx, reg_cx};
  // arg_registers[5] = {reg_di, reg_si, reg_dx, reg_cx, reg_8};
  // arg_registers[6] = {reg_di, reg_si, reg_dx, reg_cx, reg_8, reg_9};
}