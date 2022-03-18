#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// #include <L2.h>
#include <code_generator.h>
#include <L2_parser.h>

// included libraries
#include <unordered_set>
#include <tuple>

namespace L2 {

  void Gen_Code_Visitors::VisitInstruction (Instruction_ret *element) {
    this->outputFile << "\t" << "return" << std::endl;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_assignment *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_arithmetic *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_crement *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_shift *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_cmp *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_cjump *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_lea *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_calls *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_runtime *element) {
    this->outputFile << "\t" << element->toString() << std::endl;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_label *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_goto *element) {
    return;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_stackarg *element) {
    return;
  }

  Gen_Code_Visitors::Gen_Code_Visitors(std::ofstream &outF) : outputFile(outF) {
    
  }

  void generate_L1_file(Program p) {
    /* Create Output File */
    std::ofstream outputFile;
    outputFile.open("prog.L1");

    /* Create Program Header */
    outputFile << "(" << p.entryPointLabel << std::endl;

    for (auto f : p.functions) {
      /* Create function header */
      Gen_Code_Visitors v {outputFile};
      // outputFile << "\t" << newfunc->name << std::endl;
      // outputFile << "\t\t" << newfunc->arguments << " " << newfunc->locals << std::endl;

      /* Call visitor pattern */

      for (auto i : f->instructions) {
        i->Accept(&v);
      }

      /* Create function tail */
      outputFile << "\t" << ")" << std::endl;
    }

    /* Create Program tail */
    outputFile << ")" << std::endl;
  }
}