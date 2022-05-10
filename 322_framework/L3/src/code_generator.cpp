#include <string>
#include <iostream>
#include <fstream>

#include <L3.h>
#include <code_generator.h>

// using namespace std;

extern bool shouldPrint;

namespace L3 {

  /* Visitor Constructor */

  L3_Visitors::L3_Visitors(Function *f, std::ofstream &ofs) : f(f), outputFile(ofs) {}

  /* Visitor Functions */

  void L3_Visitors::VisitInstruction(Instruction_assignment *element) {
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
  void L3_Visitors::VisitInstruction(Instruction_op *element) {
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
  void L3_Visitors::VisitInstruction(Instruction_load *element) {
    return;
  }
  void L3_Visitors::VisitInstruction(Instruction_store *element) {
    return;
  }
  void L3_Visitors::VisitInstruction(Instruction_call *element) {
    return;
  }
  void L3_Visitors::VisitInstruction(Instruction_call_assign *element) {
    return;
  }
  void L3_Visitors::VisitInstruction(Instruction_label *element) {
    return;
  }
  void L3_Visitors::VisitInstruction(Instruction_br_label *element) {
    outputFile << "\t" << "return" << std::endl;
  }
  void L3_Visitors::VisitInstruction(Instruction_br_t *element) {
    outputFile << "\t" << "return" << std::endl;
  }
  void L3_Visitors::VisitInstruction(Instruction_return *element) {
    outputFile << "\t" << "return" << std::endl;
  }
  void L3_Visitors::VisitInstruction(Instruction_return_t *element) {
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
    outputFile.open("prog.L2");

    for (auto f : p.functions) {

      auto l3_visitors = new L3_Visitors(f, outputFile);
      /* function head */
      outputFile << "define " << f->name;

      /* args */
      outputFile << " (";

      outputFile << "){\n";

      /* Instructions */
      for (auto i : f->instructions) {
        i->Accept(l3_visitors);
      }

      /* end of function */
      outputFile << "}\n\n";
    }
    
  }

}