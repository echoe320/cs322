#pragma once
#include <string>

#include "L3.h"

using namespace std;
namespace L3 {
  /* Context */
  class Context{
    public:
    bool isEmpty();
    vector<Instruction *> instructions;
    int64_t start;
    int64_t end;
  };

  vector<Context *> identify_context(Function *f);

  /* Node */
  class Node {
    public: 
      Node(); 
      Node(Item* item); 
      bool isroot = true;
      Item* val = nullptr; 
      Operation* op = nullptr;
      Node* oprand1 = nullptr; 
      Node* oprand2 = nullptr; 
  };

  /* Trees */
  class Trees_Visitor : public Visitor{
    public: 
      Trees_Visitor(Instruction* i); 
      bool isEmpty();

      unordered_set<Item*> uses; 
      unordered_set<Item*> define;
      Node* root;
      Instruction* instruction;

      void VisitInstruction(Instruction_ret_not *i) override;
      void VisitInstruction(Instruction_ret_t *i) override;
      void VisitInstruction(Instruction_assignment *i) override;
      void VisitInstruction(Instruction_load *i) override;
      void VisitInstruction(Instruction_arithmetic *i) override;
      void VisitInstruction(Instruction_store *i) override;
      void VisitInstruction(Instruction_cmp *i) override;
      void VisitInstruction(Instruction_br_label *i) override;
      void VisitInstruction(Instruction_br_t *i) override;
      void VisitInstruction(Instruction_call_noassign *i) override;
      void VisitInstruction(Instruction_call_assignment *i) override;
      void VisitInstruction(Instruction_label *i) override;
  };

  /* Tiles */
  // TODO

  void select_instruction(Program p, Function* f);
}