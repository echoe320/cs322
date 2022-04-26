#include <string>
#include <iostream>
#include <fstream>

#include <IR.h>
#include <code_generator.h>

// using namespace std;

extern bool shouldPrint;

namespace IR {

  /* Visitor Constructor */

  // IR_Visitors::IR_Visitors(Function *f, std::ofstream &s) {
  //   this->outputFile = s;
  //   this->f = f;
  // }

  IR_Visitors::IR_Visitors(Function *f, std::ofstream &ofs) : f(f), outputFile(ofs) {}

  /* Visitor Functions */

  void IR_Visitors::VisitInstruction(Instruction_def *element) { //skip
    return;
  }
  void IR_Visitors::VisitInstruction(Instruction_assignment *element) {
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto src = std::get<1>(fields);

    auto dst_temp = dynamic_cast<Variable *>(dst);

    // auto src_temp;
    
    if (dynamic_cast<Variable *>(src) != nullptr) {
      src = dynamic_cast<Variable *>(src);
    } else if (dynamic_cast<Label *>(src) != nullptr)
    {
      src = dynamic_cast<Label *>(src);
    } else if (dynamic_cast<Number *>(src) != nullptr)
    {
      src = dynamic_cast<Number *>(src);
    }
    
    outputFile << "\t" << dst_temp->toString() << " <- " << src->toString() << std::endl;
  }
  void IR_Visitors::VisitInstruction(Instruction_op *element) {
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto arg1 = std::get<1>(fields);
    auto op = std::get<2>(fields);
    auto arg2 = std::get<3>(fields);

    auto dst_temp = dynamic_cast<Variable *>(dst);

    auto op_temp = dynamic_cast<Operation *>(op);
    auto op_val = op_temp->get();
    
    if (dynamic_cast<Variable *>(arg1) != nullptr) {
      arg1 = dynamic_cast<Variable *>(arg1);
    } else if (dynamic_cast<Number *>(arg1) != nullptr)
    {
      arg1 = dynamic_cast<Number *>(arg1);
    }

    if (dynamic_cast<Variable *>(arg2) != nullptr) {
      arg2 = dynamic_cast<Variable *>(arg2);
    } else if (dynamic_cast<Number *>(arg2) != nullptr)
    {
      arg2 = dynamic_cast<Number *>(arg2);
    }
    
    outputFile << "\t" << dst_temp->toString() << " <- " << arg1->toString() << get_enum_string(op_val) << arg2->toString() << std::endl;
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
    outputFile << "\t" << "return" << std::endl;
  }
  void IR_Visitors::VisitInstruction(te_br_t *element) {
    outputFile << "\t" << "return" << std::endl;
  }
  void IR_Visitors::VisitInstruction(te_return *element) {
    outputFile << "\t" << "return" << std::endl;
  }
  void IR_Visitors::VisitInstruction(te_return_t *element) {
    auto fields = element->get();
    auto arg = std::get<0>(fields);
    
    if (dynamic_cast<Variable *>(arg) != nullptr) {
      arg = dynamic_cast<Variable *>(arg);
    } else if (dynamic_cast<Number *>(arg) != nullptr)
    {
      arg = dynamic_cast<Number *>(arg);
    }

    outputFile << "\t" << "return " << arg->toString() << std::endl;
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

      /* args */
      outputFile << " (";

      outputFile << "){\n";

      /* Instructions */
      for (auto b : f->basicblocks) {
        // continue;
        for (auto i : b->instructions) {
          i->Accept(ir_visitors);
        }
      }

      /* end of function */
      outputFile << "}\n\n";
    }
    
  }

}