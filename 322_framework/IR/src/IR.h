#pragma once

#include <vector>
#include <string>

#include <unordered_set>
#include <set>
#include <map>

namespace IR {
  /*
   * Item Objects
   */

  enum var_type {int64, int64_arr, tup, code};

  enum func_type {int64_f, int64_arr_f, tup_f, code_f, v_f};

  enum opCode {op_add, op_minus, op_multiply, op_AND, op_lshift, op_rshift, cmp_lesseq, cmp_greateq, cmp_less, cmp_great, cmp_equals};

  static const char* op_enum_str[] = {" + ", " - ", " * ", " & ", " << ", " >> ", " <= ", " >= ", " < ", " > ", " = "};

  enum calleeCode {cc_print, cc_input, cc_tensor_error};

  static const char* callee_str[] = {"print", "input", "tensor-error"};

  std::string get_enum_string (int enum_value);

  std::string get_callee (int enum_value);
  
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
      Variable(var_type tp, std::string vn);
      std::pair<var_type, std::string> get(void);
      std::string toString(void) override;
      var_type toType(void);
      int dim = 0;
    private:
      var_type varType;
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

  class Array : public Variable {
    public:
      Array(std::string name, int dim);
      int64_t dimension;
  };

  class Tuple: public Variable {
    public:
      Tuple(std::string name);
  };

  class Callee : public Item {
    public:
      Callee(calleeCode cc);
      calleeCode get(void);
      std::string toString(void) override;
    private:
      calleeCode ce;
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
  class Instruction_def : public Instruction {
    public:
      Instruction_def(Item *t_v);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get(); //access individual fields by std::get<idx>(element.get())
      std::string typeAsString(void) override;
    private:
      Item *var; // src = std::get<0>(element.get());
  };

  class Instruction_assignment : public Instruction{
    public:
      Instruction_assignment(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get(); //access individual fields by std::get<idx>(element.get())
      std::string typeAsString(void) override;
    private:
      Item *dst, *src; // src = std::get<0>(element.get());
  };

  class Instruction_op : public Instruction{
    public:
      Instruction_op(Item *dest, Item *a1, Item *a2, Item *ope);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
    private:
      Item *dst, *arg1, *arg2, *op;
  };

  class Instruction_load : public Instruction{
    public:
      Instruction_load(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
    private:
      Item *dst, *src;
  };

  class Instruction_store : public Instruction{
    public:
      Instruction_store(Item *dest, Item *source);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*> get();
      std::string typeAsString(void) override;
    private:
      Item *dst, *src;
  };

  class Instruction_length : public Instruction{
    public:
      Instruction_length(Item *dest, Item *source, Item *d);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *dst, *src, *dim;
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

  class Instruction_array : public Instruction {
    public:
      Instruction_array(Item *dest, std::vector<Item*> as);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, std::vector<Item*>> get();
      std::string typeAsString(void) override;
      Item* dst; 
      std::vector<Item*> args;
  };

  class Instruction_tuple: public Instruction {
    public:
      Instruction_tuple(Item *dest, Item * as);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item *> get();
      std::string typeAsString(void) override;
      Item* dst, * arg;
  };

  class Instruction_label : public Instruction{
    public:
      Instruction_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
    private:
      Item *label;
  };

  // te instructions

  class te_br_label : public Instruction {
    public:
      te_br_label(Item *target);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
      Item *label;
  };

  class te_br_t : public Instruction {
    public:
      te_br_t(Item *tar, Item *l1, Item *l2);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*, Item*, Item*> get();
      std::string typeAsString(void) override;
      Item *t, *label1, *label2;
  };

  class te_return : public Instruction{
    public:
      te_return();
      void Accept(Visitor *visitor) override;
      std::string typeAsString(void) override;
  };

  class te_return_t : public Instruction{
    public:
      te_return_t(Item *tar);
      void Accept(Visitor *visitor) override;
      std::tuple<Item*> get();
      std::string typeAsString(void) override;
    private:
      Item *t;
  };

  /*
   * Instruction interface.
   */
  class Visitor {
    public:
      virtual void VisitInstruction(Instruction_def *element) = 0;
      virtual void VisitInstruction(Instruction_assignment *element) = 0;
      virtual void VisitInstruction(Instruction_op *element) = 0;
      virtual void VisitInstruction(Instruction_load *element) = 0;
      virtual void VisitInstruction(Instruction_store *element) = 0;
      virtual void VisitInstruction(Instruction_length *element) = 0;
      virtual void VisitInstruction(Instruction_call *element) = 0;
      virtual void VisitInstruction(Instruction_call_assign *element) = 0;
      virtual void VisitInstruction(Instruction_array *element) = 0;
      virtual void VisitInstruction(Instruction_tuple *element) = 0;
      virtual void VisitInstruction(Instruction_label *element) = 0;
      virtual void VisitInstruction(te_br_label *element) = 0;
      virtual void VisitInstruction(te_br_t *element) = 0;
      virtual void VisitInstruction(te_return *element) = 0;
      virtual void VisitInstruction(te_return_t *element) = 0;
  };

  /*
   * Basic_Block
   */

  class Basic_Block {
    public:
    Label* label; 
    std::vector<Instruction*> instructions;
    Instruction* te; 
  };

  /*
   * Function
   */
  class Function{
    public:
      func_type type; 
      std::string name;
      bool isMain;
      std::vector<Variable *> arguments;
      std::vector<Basic_Block *> basicblocks;
      std::map<std::string, Variable *> existing_vars;
  };

  /*
   * Program.
   */
  class Program{
    public:
      // std::string entryPointLabel;
      std::vector<Function *> functions;
  };
}