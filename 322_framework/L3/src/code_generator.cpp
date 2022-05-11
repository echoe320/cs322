#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <L3.h>
#include <code_generator.h>
#include "instruction_select.h"

// using namespace std;

bool shouldPrint;

namespace L3 {
  int arg_reg_list[] = {L3::reg::rdi, L3::reg::rsi, L3::reg::rdx, L3::reg::rcx, L3::reg::r8, L3::reg::r9};

  bool comp_label(Label *l1, Label *l2) {
    return l1->get().length() > l2->get().length();
  }

  void label_global(Program *p) { 
    //find longest label for whole L3 program
    std::vector<Label *> labels;
    for (auto f : p->functions) {
      for (auto i : f->instructions) {
        Instruction_label *l = dynamic_cast<Instruction_label *>(i);
        if (l) {
          labels.push_back(dynamic_cast<Label *>(l->label));
        }
      }
    }
    p->LL = "";
    if (labels.size() == 0) return;
    sort(labels.begin(), labels.end(), comp_label);
    // Label *l_temp = dynamic_cast<Label *>(labels[0]);
    std::string llg = dynamic_cast<Label *>(labels[0])->get().substr(1);
    p->LL = llg;
    llg = llg + "_global_";

    //change label to LLG
    for (auto f : p->functions) {
      std::string fname = f->name.substr(1);
      for (auto i : f->instructions) {
        Instruction_label *l = dynamic_cast<Instruction_label *>(i);
        if (l) {
          std::string old_lab = dynamic_cast<Label *>(l->label)->get();
          l->label = new Label(":" + llg + fname + "_" + old_lab.substr(1));
          if(shouldPrint) std::cout << "label: " << old_lab << std::endl;
        }
        Instruction_br_label *bl = dynamic_cast<Instruction_br_label *>(i);
        if(bl) {
          std::string old_lab = dynamic_cast<Label *>(bl->label)->get();
          bl->label = new Label(":" + llg + fname + "_" + old_lab.substr(1));
          if(shouldPrint) std::cout << "br label: " << old_lab << std::endl;                    
        }
        Instruction_br_t *brt = dynamic_cast<Instruction_br_t *>(i);
        if(brt) {
          std::string old_lab = dynamic_cast<Label *>(brt->label)->get().substr(1);
          brt->label = new Label(":" + llg + fname + "_" + old_lab.substr(1));
          if(shouldPrint) std::cout << "brt label: " << old_lab << std::endl;                    
        }
        Instruction_assignment *a = dynamic_cast<Instruction_assignment *>(i);
        if(a) {
          Label* label = dynamic_cast<Label*>(a->src);
          if(label) {
            std::string old_lab = label->get();
            label = new Label(":" + llg + fname + "_" + old_lab.substr(1));
            if(shouldPrint) std::cout << "assign label: " << old_lab << std::endl; 
          }                   
        }
      }
    }
  }

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
    /* global labels */    
    label_global(&p);

    /* Open and initialize the output file */
    std::ofstream outputFile;
    outputFile.open("prog.L2");
    outputFile << "(:main" << std::endl; 

    /* iterate through functions */
    for (auto f : p.functions) {
      auto l3_visitors = new L3_Visitors(f, outputFile);
      /* function head */
      outputFile << "\t(" << f->name << std::endl;
      outputFile << "\t\t" << f->arguments.size() << std::endl;
      
      /* args */
      if (f->arguments.size()) {
        // auto arg_regs = L2::Architecture::get_argument_regs();
        int idx = 0;
        for (auto arg : f->arguments) {
          std::string line;
          int size = f->arguments.size();
          int64_t stack_pos = (size - idx - 1) * 8;
          if (idx >= 6) {
            line = "\t" + arg->toString() + "<-" + "stack-arg " + std::to_string(stack_pos) + "\n";
          } else {
            line = "\t" + arg->toString() + " <- " + get_reg_string(arg_reg_list[idx]) + "\n";
          }
          // all_instructions.push_back(line);
          outputFile << "\t" << line;
          idx++;
        }
      }

      /* Instruction select */
      // want to give list of tiled functions to transform
      
      // for (auto i : f->instructions) {
      //   i->Accept(l3_visitors);
      // }
      std::vector<std::string> L2_inst = inst_select(p, f);

      /* end of function */
      outputFile << "\t)\n\n";
    }
    outputFile << ")\n\n";
    
  }

}