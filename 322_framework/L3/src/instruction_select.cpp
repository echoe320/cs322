#include <string> 
#include <iostream>
#include <algorithm>

#include "instruction_select.h"
#include "L3.h"
#include "code_generator.h"
#include "liveness.h"

namespace L3 {
  /* Context class functions */
  bool Context::isEmpty() { 
    return this->instructions.size() == 0;
  }

  std::vector<Context *> identifyContext(Function *f) {
    std::vector<Context *> contexts;
    Context *C = new Context();
    C->start = 0;

    for (int idx = 0; idx < f->instructions.size(); idx++) {
      Instruction *i = f->instructions[idx];
      Instruction_label *l = dynamic_cast<Instruction_label *>(i);
      Instruction_call *c = dynamic_cast<Instruction_call *>(i);
      Instruction_br_label *bl = dynamic_cast<Instruction_br_label *>(i);
      Instruction_br_t *brt = dynamic_cast<Instruction_br_t *>(i);
      Instruction_return *r = dynamic_cast<Instruction_return *>(i);
      Instruction_return_t *rt = dynamic_cast<Instruction_return_t *>(i);
      
      if (!(l || c)) {
        C->instructions.push_back(i);
      }
      
      if (l || c || bl || brt || r || rt) {
        if (!C->isEmpty()) {
          if (l || c) {
            C->end = idx;
          } else {
            C->end = idx + 1;
          }
          contexts.push_back(C);
        }
        C = new Context();
        C->start = idx + 1;
      }
    }
    return contexts;
  }

  /* Node class functions */
  Node::Node(Item* i) {
    this->item = i;
  }

  /* Tree class functions */
  // Tree::Tree(Instruction* i) {
  //   this->instruction = i;
  // }


  std::vector<std::string> inst_select(Program p, Function* f){
    std::vector<std::string> tiled_instructions;

    /* Liveness */
    GenKill genkill; 
    for(auto i : f->instructions) {
      i->Accept(&genkill);
    }
    AnalysisResult* res = computeLiveness(f);

    /* Tiling */

    /* Context */

    std::vector<Context*> gen_context = identifyContext(f);

    return tiled_instructions;
  }
}