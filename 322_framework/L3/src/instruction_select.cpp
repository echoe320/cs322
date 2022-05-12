#include <string> 
#include <iostream>
#include <algorithm>

#include "L3.h"
#include "code_generator.h"
#include "tiling.h"
#include "instruction_select.h"
#include "liveness.h"
using namespace std; 

extern bool shouldPrint;

namespace L3 {
  // Context
  bool Context::isEmpty() { 
    return instructions.size() == 0; 
  }

  vector<Context *> identify_context(Function *f) {
    vector<Context *> contexts;
    Context *C = new Context();
    C->start = 0;
    int ii = 0;
    while (ii < f->instructions.size()) {
      auto temp_inst = f->instructions[ii];
      Instruction_label *l = dynamic_cast<Instruction_label *>(temp_inst);
      Instruction_call *c = dynamic_cast<Instruction_call *>(temp_inst);
      Instruction_br *b = dynamic_cast<Instruction_br *>(temp_inst);
      Instruction_ret *r = dynamic_cast<Instruction_ret *>(temp_inst);
      
      if (l == nullptr && c == nullptr) {
        C->instructions.push_back(temp_inst);
      }
      
      if (l || c || b || r) {
        if (!C->isEmpty()) {
          if (l || c) {
            C->end = ii;
          } else {
            C->end = ii + 1;
          }
          contexts.push_back(C);
        }
        C = new Context();
        C->start = ii + 1;
      }
      ii++;
    }
    return contexts;
  }

  // Tree Node
  constexpr int count = 10;
  Node::Node(Item* item) {
      val = item;
  }
  Node::Node() {
      val = nullptr;
  }

  Trees_Visitor::Trees_Visitor(Instruction* i) {
    this->instruction = i;
  }
  Instruction* Trees_Visitor::getInstruction() {
    return this->instruction;
  }

  void Trees_Visitor::VisitInstruction(Instruction_ret_not* i) {
    Node* node = new Node(new Empty());
    node->op = i->op;
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_ret_t* i) {
    Node* node = new Node(new Empty());
    node->op = i->op;
    node->oprand1 = new Node(i->arg); 
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_assignment* i) {
    Node* node = new Node(i->dst);
    node->op = new Operation("<-");
    node->oprand1 = new Node(i->src); 
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_arithmetic* i) {
    Node* node = new Node(i->dst); 
    node->oprand1 = new Node(i->oprand1); 
    node->op = dynamic_cast<Operation*>(i->op); 
    node->oprand2 = new Node(i->oprand2);
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_cmp* i) {
    Node* node = new Node(i->dst); 
    node->oprand1 = new Node(i->oprand1); 
    node->op = dynamic_cast<Operation*>(i->op); 
    node->oprand2 = new Node(i->oprand2);
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_load* i) {
    Node* node = new Node(i->dst); 
    node->oprand1 = new Node(i->src); 
    node->op = dynamic_cast<Operation*>(i->op); 
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_store* i) {
    Node* node = new Node(i->dst); 
    node->oprand1 = new Node(i->src); 
    node->op = dynamic_cast<Operation*>(i->op);  
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_br_t* i) {
    Node* node = new Node(new Empty()); 
    node->op = i->op;
    node->oprand1 = new Node(i->condition); 
    node->oprand2 = new Node(i->label); 
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_br_label* i) {
    Node* node = new Node(new Empty()); 
    node->op = i->op;
    node->oprand1 = new Node(i->label); 
    root = node;
    uses = i->uses; 
    define = i->define;
  }
  void Trees_Visitor::VisitInstruction(Instruction_call_noassign *i) {}
  void Trees_Visitor::VisitInstruction(Instruction_call_assignment *i) {}
  void Trees_Visitor::VisitInstruction(Instruction_label *i) {}

  vector<Trees_Visitor*> getAllTree(Context* context){
    vector<Trees_Visitor*> trees; 
    for (auto i : context->instructions) {
      Trees_Visitor* t = new Trees_Visitor();
      i->accept(t);
      trees.push_back(t);
    }
    return trees;
  }
  vector<Trees_Visitor*> mergeTrees(Context* context){
    //TODO
  }

  vector<std::string> select_instruction(Program p, Function* f){   
    //TODO: identify context, compute liveness, translate context into trees, merge trees, match trees to tiles 
  }
}