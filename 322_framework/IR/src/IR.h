#pragma once

#include <vector>
#include <string>

#include <unordered_set>
#include <set>

namespace IR {
  class Item {
    public:
      virtual std::string toString(void) = 0;
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      //int64_t locals; //! FUNCTION DOES NOT HAVE LOCALS IN L2
      std::vector<Instruction *> instructions;
      //Initialize vectors
      //! PROBLEM: these sets (GEN, KILL, IN, OUT) are unique to each function, but we only have one of each for a program
      std::vector<std::unordered_set<Item *>> GEN; //* GEN[i] = all variables read by instruction i
      std::vector<std::unordered_set<Item *>> KILL;//* KILL[i] = all variables written/defined by instruction i
      std::vector<std::unordered_set<Item *>> IN;
      std::vector<std::unordered_set<Item *>> OUT;
      void findSuccessorsPredecessors(); // could be a vistor pattern function, take a look after finish
  };

  /*
   * Program.
   */
  class Program{
    public:
      std::string entryPointLabel;
      std::vector<Function *> functions;
  };
}