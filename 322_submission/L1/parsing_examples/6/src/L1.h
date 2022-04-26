#pragma once

#include <vector>
#include <string>

namespace L1 {

  class Item {
    public:
      std::string labelName;
  };

  /*
   * Instruction interface.
   */
  class Instruction{
  };

  /*
   * Instructions.
   */
  class Instruction_ret : public Instruction{
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      int64_t locals;
      std::vector<Instruction *> instructions;
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
