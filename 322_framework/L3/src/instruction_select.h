#pragma once
#include <string>
#include "L3.h"
#include <architecture.h>
using namespace std;
namespace L3 {
  class Tile;
  class Context{
    public:
    bool isEmpty();
    vector<Instruction *> instructions;
    int64_t start;
    int64_t end;
  };

  vector<Context *> identify_context(Function *f);
  class Node {
    public: 
      bool isroot = true;
      Node(); 
      Node(Item* item); 
      Item* val = nullptr; 
      Operation* op = nullptr;
      Node* oprand1 = nullptr; 
      Node* oprand2 = nullptr; 
      void printNode(Node* node, int space);
  };
  class Trees_Visitor : public Visitor{
    public:
      vector<Tile *> tiles;
      Node* root; 
      Trees_Visitor(Instruction* i); 
      bool isEmpty() {return root == nullptr;};
      vector<Item*> uses; 
      vector<Item*> define;
      vector<string> L2_instructions;
      Instruction* getInstruction();
      void printTree(Trees_Visitor* tree);

      void visit(Instruction_ret_not *i) override;
      void visit(Instruction_ret_t *i) override;
      void visit(Instruction_assignment *i) override;
      void visit(Instruction_load *i) override;
      void visit(Instruction_math *i) override;
      void visit(Instruction_store *i) override;
      void visit(Instruction_compare *i) override;
      void visit(Instruction_br_label *i) override;
      void visit(Instruction_br_t *i) override;
      void visit(Instruction_call_noassign *i) override;
      void visit(Instruction_call_assignment *i) override;
      void visit(Instruction_label *i) override;
    private: 
      Instruction* instruction;
  };
  vector<string> inst_select(Program p, Function* f);
}