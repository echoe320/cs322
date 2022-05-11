#pragma once
#include <string>
#include "L3.h"

namespace L3 {
  /* Context section */
  class Context {
    public:
      std::vector<Instruction *> instructions;
      int64_t start, end;
      bool isEmpty();
  };
  std::vector<Context *> identifyContext(Function *f);

  /* Tree section */
  class Node {
    public: 
      Node(); 
      Node(Item* i); 
      bool istop = true;
      Item* val = nullptr; 
      Operation* op = nullptr;
      Node* oprand1 = nullptr; 
      Node* oprand2 = nullptr; 
      void printNode(Node* node, int space);
    private:
      Item* item;
  };

  class Tree : public Visitor{
    public:
      Tree(Instruction* i); 
      // std::vector<Tile *> tiles;
      Node* top; 
      bool isEmpty() {return top == nullptr;};
      std::vector<Item*> uses; 
      std::vector<Item*> define;
      std::vector<std::string> L2_instructions;
      Instruction* getInstruction();
      void printTree(Tree* tree);

      void VisitInstruction(Instruction_assignment *element) override;
      void VisitInstruction(Instruction_op *element) override;
      void VisitInstruction(Instruction_load *element) override;
      void VisitInstruction(Instruction_store *element) override;
      void VisitInstruction(Instruction_call *element) override;
      void VisitInstruction(Instruction_call_assign *element) override;
      void VisitInstruction(Instruction_label *element) override;
      void VisitInstruction(Instruction_br_label *element) override;
      void VisitInstruction(Instruction_br_t *element) override;
      void VisitInstruction(Instruction_return *element) override;
      void VisitInstruction(Instruction_return_t *element) override;

      // void visit(Instruction_ret_not *i) override;
      // void visit(Instruction_ret_t *i) override;
      // void visit(Instruction_assignment *i) override;
      // void visit(Instruction_load *i) override;
      // void visit(Instruction_math *i) override;
      // void visit(Instruction_store *i) override;
      // void visit(Instruction_compare *i) override;
      // void visit(Instruction_br_label *i) override;
      // void visit(Instruction_br_t *i) override;
      // void visit(Instruction_call_noassign *i) override;
      // void visit(Instruction_call_assignment *i) override;
      // void visit(Instruction_label *i) override;
    
    private: 
      Instruction* instruction;
  };
  /* Tiling */
  

  /* Main Instruction select function */
  std::vector<std::string> inst_select(Program p, Function* f);
}