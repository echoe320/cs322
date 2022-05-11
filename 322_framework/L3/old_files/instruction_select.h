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
  class Tree : public Visitor {

  };

  /* Tiling */
  

  /* Main Instruction select function */
  std::vector<std::string> inst_select(Program p, Function* f);
}