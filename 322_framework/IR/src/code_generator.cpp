#include <string>
#include <iostream>
#include <fstream>

#include <IR.h>
#include <code_generator.h>

// using namespace std;

extern bool shouldPrint;

namespace IR {

  /* Visitor Constructor */

  IR_Visitors::IR_Visitors(Function *f, std::ofstream &s) {
    this->outputFile = s;
    this->f = f;
  }

  /* Visitor Functions */

  void IR_Visitors::VisitInstruction(Instruction_def *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_assignment *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_op *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_load *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_store *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_length *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_call *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_call_assign *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_array *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_tuple *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_label *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(te_br_label *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(te_br_t *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(te_return *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(te_return_t *element) {
    return;
  }

  void generate_code(Program p) {

    /* 
     * Open the output file.
     */
    
    std::ofstream outputFile;
    outputFile.open("prog.L3");

    for (auto f : p.functions) {

      auto ir_visitors = new IR_Visitors(f, outputFile);
      /* function head */
      outputFile << "define " << f->name;

      // args
      outputFile << " (";

      outputFile << "){\n";

      for (auto b : f->basicblocks) {
        // continue;
        for (auto i : b->instructions) {
          i->Accept(ir_visitors);
        }
      }

      /* end of function */
      outputFile << "}";
    }
    
  }

}