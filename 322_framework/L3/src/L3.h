#pragma once

#include <vector>
#include <string>

#include <unordered_set>
#include <set>
#include <map>

namespace L3 {
  /*
   * Item Objects
   */

  enum var_type {int64, int64_arr, tup, code};

  enum func_type {int64_f, int64_arr_f, tup_f, code_f, v_f};

  enum opCode {op_add, op_minus, op_multiply, op_AND, op_lshift, op_rshift, cmp_lesseq, cmp_greateq, cmp_less, cmp_great, cmp_equals};

  static const char* op_enum_str[] = {" + ", " - ", " * ", " & ", " << ", " >> ", " <= ", " >= ", " < ", " > ", " = "};

  enum calleeCode {cc_print, cc_allocate, cc_input, cc_tensor_error};

  enum reg {rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};

  static const char* reg_enum_str[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsp"};

  std::string get_enum_string (int enum_value);
  std::string get_reg_string (int enum_value);
  
  class Item {
    public:
      virtual std::string toString(void) = 0;
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

  class Callee : public Item {
    public:
      Callee(calleeCode cc);
      calleeCode get(void);
      std::string toString(void) override;
    private:
      calleeCode ce;
  };

  class Empty : public Item {
    public:
      Empty(); 
      std::string toString() override {return "";};
      // ItemType getType(void) override { return item_empty; }
  };

  // Declare Visitor first
  class Visitor;

  /*
   * Instruction objects
   */

  class Instruction {
    public:
      virtual void Accept(Visitor *visitor) = 0;
      virtual std::string typeAsString(void) = 0;
  };

  // Regular Instructions

  class Instruction_assignment : public Instruction{
    public:
      Instruction_assignment(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get(); //access individual fields by std::get<idx>(element.get())
      std::string typeAsString(void) override;
      Item *dst, *src; // src = std::get<0>(element.get());
  };

  class Instruction_op : public Instruction{
    public:
      Instruction_op(Item *dest, Item *a1, Item *a2, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *dst, *arg1, *arg2, *op;
  };

  class Instruction_load : public Instruction{
    public:
      Instruction_load(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *dst, *src;
  };

  class Instruction_store : public Instruction{
    public:
      Instruction_store(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *dst, *src;
  };

  class Instruction_call : public Instruction{
    public:
      Instruction_call(Item *call, std::vector<Item*> as);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, std::vector<Item*>> get();
      std::string typeAsString(void) override;
      Item *callee;
      std::vector<Item*> args;
  };

  class Instruction_call_assign : public Instruction{
    public:
      Instruction_call_assign(Item *dest, Item *call, std::vector<Item*> as);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, std::vector<Item*>> get();
      std::string typeAsString(void) override;
      Item *dst, *callee;
      std::vector<Item*> args;
  };

  class Instruction_label : public Instruction{
    public:
      Instruction_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      Item *label;
  };

  // te instructions

  class Instruction_br_label : public Instruction {
    public:
      Instruction_br_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      Item *label;
  };

  class Instruction_br_t : public Instruction {
    public:
      Instruction_br_t(Item *tar, Item *l1);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *t, *label;
  };

  class Instruction_return : public Instruction{
    public:
      Instruction_return();
      void Accept(Visitor *visitor) override;
      std::string typeAsString(void) override;
  };

  class Instruction_return_t : public Instruction{
    public:
      Instruction_return_t(Item *tar);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      Item *t;
  };

  /*
   * Instruction interface.
   */
  class Visitor {
    public:
      virtual void VisitInstruction(Instruction_assignment *element) = 0;
      virtual void VisitInstruction(Instruction_op *element) = 0;
      virtual void VisitInstruction(Instruction_load *element) = 0;
      virtual void VisitInstruction(Instruction_store *element) = 0;
      virtual void VisitInstruction(Instruction_call *element) = 0;
      virtual void VisitInstruction(Instruction_call_assign *element) = 0;
      virtual void VisitInstruction(Instruction_label *element) = 0;
      virtual void VisitInstruction(Instruction_return *element) = 0;
      virtual void VisitInstruction(Instruction_return_t *element) = 0;
      virtual void VisitInstruction(Instruction_br_label *element) = 0;
      virtual void VisitInstruction(Instruction_br_t *element) = 0;
  };

  /*
   * Function
   */
  class Function{
    public:
      std::string name;
      // bool isMain;
      std::vector<Item *> arguments;
      std::vector<Instruction *> instructions;
      std::map<std::string, Variable *> existing_vars;
  };

  /*
   * Program.
   */
  class Program{
    public:
      // std::string entryPointLabel;
      std::vector<Function *> functions;
      std::string LL;
  };
}