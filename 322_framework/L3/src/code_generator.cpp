#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

#include "code_generator.h"
#include "instruction_select.h"

extern bool is_debug; 
namespace L3{
  int arg_reg_list[] = {L3::reg::rdi, L3::reg::rsi, L3::reg::rdx, L3::reg::rcx, L3::reg::r8, L3::reg::r9};

  bool comp_label(Label *l1, Label *l2) {
    return l1->get().length() > l2->get().length();
  }

  void label_global(Program *p) {
    std::vector<Label *> labels;
    for (auto f : p->functions) {
      for (auto i : f->instructions) {
        Instruction_label *l = dynamic_cast<Instruction_label *>(i);
        if (l) {
          labels.push_back(l->label);
        }
      }
    }
    p->ll = "";

    if (labels.size() == 0) return;
    std::sort(labels.begin(), labels.end(), comp_label);

    std::string llg = labels[0]->get().substr(1);
    p->ll = llg;
    llg = llg + "_global_";

    for (auto f : p->functions) {
      std::string fname = f->name.substr(1);
      for (auto i : f->instructions) {
        Instruction_label *l = dynamic_cast<Instruction_label *>(i);
        Instruction_br_label *bl = dynamic_cast<Instruction_br_label *>(i);
        Instruction_br_t *bt = dynamic_cast<Instruction_br_t *>(i);
        Instruction_assignment *a = dynamic_cast<Instruction_assignment *>(i);
        if (l) {
          std::string temp = l->label->get();
          l->label = new Label(":" + llg + fname + "_" + temp.substr(1));
          if(is_debug) cout << "label: " << temp << endl;
        } else if(bl) {
          std::string temp = bl->label->get();
          bl->label = new Label(":" + llg + fname + "_" + temp.substr(1));
          if(is_debug) cout << "br label: " << temp << endl;                    
        } else if(bt) {
          std::string temp = bt->label->get();
          bt->label = new Label(":" + llg + fname + "_" + temp.substr(1));
          if(is_debug) cout << "bt label: " << temp << endl;                    
        } else if(a) {
          Label* label = dynamic_cast<Label*>(a->src);
          if(label) {
            std::string temp = label->get();
            label = new Label(":" + llg + fname + "_" + temp.substr(1));
            if(is_debug) cout << "assign label: " << temp << endl; 
          }                   
        }
      }
    }
  }

  string temp_var(string varname) {
      return "%tmp_" + varname.substr(1);
  }

  Gen_Code_Visitors::Gen_Code_Visitors(std::ofstream &outF) : outputFile(outF) {
    
  }

  /* Defining Visitor Functions */

  void Gen_Code_Visitors::VisitInstruction(Instruction_ret_not *element){
    
  }
  
  void Gen_Code_Visitors::VisitInstruction(Instruction_ret_t *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_assignment *element) {
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_load *element) {
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_arithmetic *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_store *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_cmp *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_br_label *element){
   
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_br_t *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_call_noassign *element){
         
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_call_assignment *element){
    
  }

  void Gen_Code_Visitors::VisitInstruction(Instruction_label *element){
    
  }

  void generate_L2_file(Program p){
    /* 
     * Open the output file and initialize
     */ 
    std::ofstream outputFile;
    outputFile.open("prog.L2");
    outputFile << "(:main\n";

    /* Globalize Labels */
    label_global(&p);

    /* Iterate through functions */
    for (auto f : p.functions) {
      /* function head */
      outputFile << "\t(" << f->name << std::endl;
      outputFile << "\t\t" << f->arguments.size() << std::endl;

      /* args */
      int arg_size = f->arguments.size();

      if (arg_size) {
        int idx = 0;
        for (auto arg : f->arguments) {
          string line;
          int64_t stack_pos = (arg_size - idx - 1) * 8;
          if (idx < 6) {
            line = arg->toString() + " <- " + get_reg_string(arg_reg_list[idx]);
          } else {
            line = arg->toString() + " <- stack-arg " + std::to_string(stack_pos);
          }
          outputFile << "\t\t" << line << std::endl;
          idx++;
        }
      }

      Gen_Code_Visitors v {outputFile};

      /* Naive Solution */
      for (auto i : f->instructions) {
        std::cout << i->toString() << std::endl;
        i->Accept(&v);
      }

      /* Instruction select TODO*/
      // vector<string> L2_instructions = L3::inst_select(p, f);
      
      // for(string s : L2_instructions) {
      //   outputFile << "\t" << s; 
      // }
      
      /* end of function */
      outputFile << "\t)\n"; 
    }
    /* close main function */
    outputFile << ")\n\n";
  }
}