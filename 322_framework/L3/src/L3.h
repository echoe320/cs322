#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

namespace L3
{

  class Visitor;

  enum ItemType {
    item_number,
    item_label,
    item_variable,
    item_string,
    item_operation,
    item_empty
  };

  class Item
  {
  public:
    virtual string toString(void) = 0;
    virtual ItemType getType(void) = 0;
  };

  class Number : public Item
  {
  public:
    Number(int64_t n); 
    int64_t get (void); 
    bool operator == (const Number &other); 
    std::string toString(void) override; 
    ItemType getType(void) override { return item_number; }
  private: 
    int64_t num;
  };

  class Label : public Item
  {
  public:
    Label(string l); 
    string get(); 
    bool operator== (const Label &other); 
    string toString(void) override; 
    ItemType getType(void) override { return item_label; }
  private: 
    string labelname;
  };

  class Variable : public Item
  {
  public:
    Variable(string varName); 
    string get (void); 
    bool operator == (const Variable &other); 
    string toString(void) override;
    ItemType getType(void) override { return item_variable; }
  private: 
    string variableName;
  };
  
  class String : public Item 
  {
    public: 
    String(string sName);
    string get(); 
    string toString() override; 
    ItemType getType(void) override { return item_string; }
    private: 
    string sName;
  };

class Operation : public Item
  {
  public:
    Operation(string op); 
    string get (void); 
    // bool operator == (const Operation &other) const; 
    string toString(void) override;
    ItemType getType(void) override { return item_operation; }
  private: 
    string op;
  };

class Empty : public Item 
  {
    public:
    Empty(); 
    string toString() override {return "";};
    ItemType getType(void) override { return item_empty; }
  };
  /*
   * Instruction interface.
   */
  class Instruction
  {
  public:
    vector<Item *> uses;
    vector<Item *> define;

    virtual void accept(Visitor* visitor) = 0; 
    virtual std::string toString() = 0; //for debug
  };

  /*
   * Instructions.
   */
  class Instruction_ret : public Instruction
  {
    public:
      Operation* op;
      virtual std::string toString() = 0;
      virtual void accept(Visitor *v) = 0;
  };

  class Instruction_ret_not : public Instruction_ret
  {
    public:
      std::string toString() override { return "return"; }
      void accept(Visitor *v) override; 
  };

  class Instruction_ret_t : public Instruction_ret
  {
    public:
      // Operation* op;
      Item* arg; 
      std::string toString() override { return "return " + arg->toString(); }
      void accept(Visitor *v) override; 
  };

  class Instruction_assignment : public Instruction
  {
  public:
    Variable* dst;
    Item* src;
    std::string toString() override { return this->dst->toString() + " <- " + this->src->toString(); }
    void accept(Visitor *v) override; 
  };

  // load instruction
  class Instruction_load : public Instruction
  {
  public:
    Variable* dst;
    Variable* src;
    Operation* op;
    std::string toString() override { 
      return this->dst->toString() + " <- load "
          + this->src->toString();
    }
    void accept(Visitor *v) override; 
  };

  // math instruction
  class Instruction_math : public Instruction
  {
  public:
    Variable *dst;
    Item *op;
    Item *oprand1;
    Item *oprand2; 
    std::string toString() override { return dst->toString() +" <- "+ oprand1->toString() + " " + op->toString() + " " + oprand2->toString(); }
    void accept(Visitor *v) override; 
  };

  // store instruction
  class Instruction_store : public Instruction
  {
  public:
    Item *src;
    Item *dst;
    Operation* op;
    std::string toString() override { return "store " + dst->toString() + " <- " + src->toString(); }
    void accept(Visitor *v) override; 
  };

  class Instruction_compare : public Instruction
  {
  public:
    Variable *dst;
    Item *oprand1;
    Item *op;
    Item *oprand2;
    std::string toString() override { return dst->toString() + " " + oprand1->toString()+ " " + op->toString()+ " " + oprand2->toString(); }
    void accept(Visitor *v) override; 
  };

  class Instruction_br : public Instruction {
    public:
    Label *label;
    Operation* op; 
    virtual std::string toString() = 0;
    virtual void accept(Visitor *v) = 0;
  };

  class Instruction_br_label : public Instruction_br
  {
  public:
    std::string toString() override { return "br " + label->toString(); }
    void accept(Visitor *v) override; 
  };

class Instruction_br_t : public Instruction_br
  {
  public:
    Item *condition;
    std::string toString() override { return "br " + condition->toString() + " " +label->toString(); }
    void accept(Visitor *v) override; 
  };

  class Instruction_call : public Instruction {
    public:
    Item *callee;
    vector<Item*> args; 
    virtual std::string toString() = 0;
    virtual void accept(Visitor *v) = 0;
  };

  // call callee (args) instruction
  class Instruction_call_noassign : public Instruction_call
  {
  public:
    std::string toString() override { 
        string s = "call " + callee->toString() + " "; 
        for(Item* i : args) s += i->toString() + " ";
        return s; 
    }
    void accept(Visitor *v) override; 
  };
  // var <- call callee (args) instruction
  class Instruction_call_assignment : public Instruction_call
  {
  public:
    Variable *dst;
    std::string toString() override { 
        string s = dst->toString() + " <- call " + callee->toString() + " "; 
        for(Item* i : args) s += i->toString() + " ";
        return s; 
    }
    void accept(Visitor *v) override; 
  };

  // label instruction
  class Instruction_label : public Instruction
  {
  public:
    Label *label;
    std::string toString() override { return label->toString(); }
    void accept(Visitor *v) override; 
  };

  /*
   * Function.
   */
  class Function
  {
  public:
    std::string name;
    bool isMain;
    vector<Item*> arguments; 
    std::vector<Instruction *> instructions;
    std::map<std::string, Variable*> variables; 
    Variable* newVariable(std::string variableName);
    Function(); 
    void format_function();
    int64_t sizeOfStack;
  };

  /*
   * Program.
   */
  class Program
  {
  public:
  //  std::string entryPointLabel;
    bool hasMain; 
    std::vector<Function *> functions;
    std::string ll;
    Program();
  //  std::map<Architecture::RegisterID, Register*> registers; 
  //  Register* getRegister(Architecture::RegisterID rid); 
  };

  class Visitor {
    public: 
      virtual void visit(Instruction_ret_not *i) = 0;
      virtual void visit(Instruction_ret_t *i) = 0;
      virtual void visit(Instruction_assignment *i) = 0;
      virtual void visit(Instruction_load *i) = 0;
      virtual void visit(Instruction_math *i) = 0;
      virtual void visit(Instruction_store *i) = 0;
      virtual void visit(Instruction_compare *i) = 0;
      virtual void visit(Instruction_br_label *i) = 0;
      virtual void visit(Instruction_br_t *i) = 0;
      virtual void visit(Instruction_call_noassign *i) = 0;
      virtual void visit(Instruction_call_assignment *i) = 0;
      virtual void visit(Instruction_label *i) = 0;
  };

}
