#pragma once

#include <vector>
#include <string>

#include <unordered_set>

namespace L2 {
  // class visitor;

  enum reg {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  std::ostream& operator<<(std::ostream& out, const Errors value){
    const char* s = 0;
    #define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
      PROCESS_VAL(rdi);     
      PROCESS_VAL(rsi);     
      PROCESS_VAL(rdx);
      PROCESS_VAL(rcx);     
      PROCESS_VAL(r8);     
      PROCESS_VAL(r9);
      PROCESS_VAL(rax);     
      PROCESS_VAL(rbx);     
      PROCESS_VAL(rbp);
      PROCESS_VAL(r10);     
      PROCESS_VAL(r11);     
      PROCESS_VAL(r12);
      PROCESS_VAL(r13);
      PROCESS_VAL(r14);     
      PROCESS_VAL(r15);     
      PROCESS_VAL(rsp);
    }
    #undef PROCESS_VAL
    return out << s;
  }
    //std::string reg [] = ["rdi", "rsi", ];

  enum opCode {op_add, op_minus, op_multiply, op_AND, op_lshift, op_rshift, op_inc, op_dec, cmp_equals, cmp_less, cmp_lesseq};
  
  enum iType {ret, assignment, arithmetic, crement, shift, cmp, cjump, lea, calls, runtime, _label, gotoo, stackarg};

  enum runtimeCode {rt_print, rt_input, rt_allocate, rt_tensor_error};

  class Item {
    public:
      // virtual void print_obj = 0;
      // std::string labelName;
      // reg r;
      // std::string Register;
      // std::string offset; // doubles as number
      //virtual void get(void) = 0;
      // virtual void get(void);
      //virtual std::string to_String(void);
      // std::vector<Item *> parsed_items;
      // Item *i; // <- add this to parser
      // reg reg_temp;
      // i = &r;
      // i->r; // reg r inside of reg_temp
      // parsed_items.push_back(i);

      // bool isARegister = false;
      // bool isMem = false;
      // bool isNum = false;
      // bool isLabel = false;
      // bool isVar = false;
      // int offset;
  };

  class Register : public Item {
    public:
      reg get(void) {
        return this->r;
      };
    private:
      reg r;
  };

  class Memory : public Item {
    public:
      std::pair<L2::reg, int64_t> get(void){
        std::pair<L2::reg, int64_t> res{this->r, this->offset};
        return res; // res.first
      };
    private:
      reg r;
      int64_t offset;
  };

  class Number : public Item {
    public:
      int64_t get(void){
        return this->num;
      };
    private:
      int64_t num;
  };

  class Label : public Item {
    public:
      std::string get(void){
        return this->labelName;
      };
    private:
      std::string labelName;
  };

  class Variable : public Item {
    public:
      std::string get(void){
        return this->varName;
      };
    private:
      std::string varName;
  };

  class Operation : public Item {
    public:
      opCode get(void){
        return this->opName;
      };
    private:
      opCode opName;
  };

  class Runtime : public Item {
    public:
      runtimeCode get(void) {
        return this->runtime;
      };
    private:
      runtimeCode runtime;
  };

  /*
   * Instruction interface.
   */
  class Visitor {
    public:
      virtual void VisitInstruction(const Instruction_ret *element) = 0;
      virtual void VisitInstruction(const Instruction_assignment *element) = 0;
      virtual void VisitInstruction(const Instruction_arithmetic *element) = 0;
      virtual void VisitInstruction(const Instruction_crement *element) = 0;
      virtual void VisitInstruction(const Instruction_shift *element) = 0;
      virtual void VisitInstruction(const Instruction_cmp *element) = 0;
      virtual void VisitInstruction(const Instruction_cjump *element) = 0;
      virtual void VisitInstruction(const Instruction_lea *element) = 0;
      virtual void VisitInstruction(const Instruction_calls *element) = 0;
      virtual void VisitInstruction(const Instruction_runtime *element) = 0;
      virtual void VisitInstruction(const Instruction_label *element) = 0;
      virtual void VisitInstruction(const Instruction_goto *element) = 0;
      virtual void VisitInstruction(const Instruction_stackarg *element) = 0;
  };

  /*
   * Instructions.
   */
  class Instruction {
    public:
      iType id;
      virtual void Accept(Visitor *visitor) = 0;
      //virtual std::string toString(void) = 0;

      std::unordered_set<Instruction *> predecessors;
      std::unordered_set<Instruction *> successors;
      std::unordered_set<Item *> reads; // Gen
      std::unordered_set<Item *> writes; // Kill
  };

  class Instruction_ret : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
  };

  class Instruction_assignment : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *src, *dst;
  };

  class Instruction_arithmetic : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *src, *dst;
      Operation op;
  };

  class Instruction_crement : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *dst;
      Operation op;
  };

  class Instruction_shift : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *src, *dst;
      Operation op;
  };

  class Instruction_cmp : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *dst, *arg1, *arg2;
      Operation op;
  };

  class Instruction_cjump : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *arg1, *arg2, *label;
      Operation op;
  };

  class Instruction_lea : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *dst, *arg1, *arg2, *multiple;
  };

  class Instruction_calls : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *u, *N; //call u N
  };

  class Instruction_runtime : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *runtime, *N;
  };

  class Instruction_label : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *label;
  };

  class Instruction_goto : public Instruction{
    public:
      void Accept(Visitor *visitor) override;
      Item *label;
  };

  class Instruction_stackarg : public Instruction {
    public:
      void Accept(Visitor *visitor) override;
      Item *dst, *M;
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