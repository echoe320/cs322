#include <string>
#include <iostream>
#include <fstream>

#include <IR.h>
#include <code_generator.h>

// using namespace std;

extern bool shouldPrint;
int count = 0;
namespace IR {

  /* Helper Functions */

  std::string decode(std::string var) {
    std::string ret = var + " <- " + var + " >> 1\n";
    return ret;
  }

  std::string encode(std::string var) {
    std::string l1 = var + " <- " + var + " << 1\n";
    std::string l2 = "\t" + var + " <- " + var + " + 1\n";
    return l1 + l2;
  }

  std::string newTempVar() {
    std::string var =  "%temp_" + std::to_string(count);
    count++;
    return var;
  }

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
    auto * d = dynamic_cast<Variable *>(element->dst);
    auto * s = dynamic_cast<Variable *>(element->src);
    Item * dim_temp;
    Variable * v = dynamic_cast<Variable *>(element->dim);
    Number * n = dynamic_cast<Number *>(element->dim);
    if (v) {
      dim_temp = v;
    } else if (n) {
      dim_temp = n;
    }
    std::string os_t = newTempVar();
    std::string dim_t = newTempVar();
    std::string ptr_t = newTempVar();
    outputFile << "\t" << os_t << " <- 16" << std::endl; 
    outputFile << "\t" << dim_t << " <- 8 * " << dim_temp->toString() << std::endl; 
    outputFile << "\t" << os_t << " <- " << os_t << " + " << dim_t << std::endl; 
    outputFile << "\t" << ptr_t << " <- " << s->toString() << " + " << os_t << std::endl;
    outputFile << "\t" << d->toString() << " <- load " << ptr_t << std::endl;
  }
  void IR_Visitors::VisitInstruction(Instruction_call *element) {
    auto args_temp = element->args;
    Item * callee_temp;
    Variable * v = dynamic_cast<Variable *>(element->callee);
    Label * n = dynamic_cast<Label *>(element->callee);
    Callee * c = dynamic_cast<Callee *>(element->callee);
    if (v) {
      callee_temp = v;
    } else if (n) {
      callee_temp = n;
    } else {
      callee_temp = c;
    }

    outputFile << "\t" << "call "<< callee_temp->toString() << " (";
    
    while(!args_temp.empty()) {
      Variable * a = dynamic_cast<Variable *>(args_temp.back());
      outputFile << a->toString();
      if (args_temp.size() > 1) {
        outputFile << ", ";
      }
      args_temp.pop_back();
    }
    outputFile << ")" << std::endl;
  }
  void IR_Visitors::VisitInstruction(Instruction_call_assign *element) {
    auto dst_temp = dynamic_cast<Variable*>(element->dst);
    auto args_temp = element->args;
    Item * callee_temp;
    Variable * v = dynamic_cast<Variable *>(element->callee);
    Label * n = dynamic_cast<Label *>(element->callee);
    Callee * c = dynamic_cast<Callee *>(element->callee);
    if (v) {
      callee_temp = v;
    } else if (n) {
      callee_temp = n;
    } else {
      callee_temp = c;
    }

    outputFile << "\t" << dst_temp->toString() << " <- call "<< callee_temp->toString() << " (";
    
    while(!args_temp.empty()) {
      Variable * a = dynamic_cast<Variable *>(args_temp.back());
      outputFile << a->toString();
      if (args_temp.size() > 1) {
        outputFile << ", ";
      }
      args_temp.pop_back();
    }
    outputFile << ")" << std::endl;
  }
  void IR_Visitors::VisitInstruction(Instruction_array *element) {
    auto args_temp = element->args;
    int64_t dimension = args_temp.size();
    // auto dst_temp = dynamic_cast<Variable *>(element->dst);
    Item * dst_temp;
      Variable * v = dynamic_cast<Variable *>(element->dst);
      Number * n = dynamic_cast<Number *>(element->dst);
      if (v) {
        dst_temp = v;
        std::cout << "dst print v" << std::endl;
      } else if (n) {
        dst_temp = n;
        std::cout << "dst print n" << std::endl;
      }

    std::string total_len_var = newTempVar();
    outputFile << "\t" << total_len_var << " <- 1\n";
    while (!args_temp.empty()) {
      std::string temp = newTempVar();
      Item * arg;
      Variable * v = dynamic_cast<Variable *>(args_temp.back());
      Number * n = dynamic_cast<Number *>(args_temp.back());
      if (v) {
        arg = v;
      } else if (n) {
        arg = n;
      }
      outputFile << "\t" << temp + " <- " + arg->toString() + "\n";
      outputFile << "\t" << decode(temp);
      outputFile << "\t" << total_len_var + " <- " + total_len_var + " * " + temp << std::endl;
      args_temp.pop_back();
    }
    outputFile << "\t" << total_len_var << " <- " << total_len_var << " + " << (dimension + 1) << std::endl;
    outputFile << "\t" << encode(total_len_var);

    std::string array_ptr_t = dst_temp->toString();
    outputFile << "\t" << array_ptr_t << " <- " << "call allocate(" << total_len_var << ", 1)" << std::endl;

    // std::string dimension_ptr = newTempVar();
    // int64_t dimension_encoded = dimension << 1;
    // dimension_encoded += 1;
    // outputFile << "\t" << dimension_ptr << " <- " << array_ptr_t << " + 8" << std::endl;
    // outputFile << "\t" << "store " << dimension_ptr << " <- " << dimension_encoded << std::endl;
    
    // for (int k = 0; k < dimension; k++) {
    //   std::string tmp_ptr = newTempVar();
    //   int64_t offset = 16 + k * 8;
    //   Item * arg;
    //   Variable * v = dynamic_cast<Variable *>(args_temp[k]);
    //   Number * n = dynamic_cast<Number *>(args_temp[k]);
    //   if (v) {
    //     arg = v;
    //   } else if (n) {
    //     arg = n;
    //   }
    //   outputFile << "\t" << tmp_ptr << " <- " << array_ptr_t << " + " << offset << std::endl;
    //   outputFile << "\t" << "store " << tmp_ptr << " <- " << arg->toString() << std::endl;
    // }
  }
  void IR_Visitors::VisitInstruction(Instruction_tuple *element) {
    auto dst_temp = dynamic_cast<Variable *>(element->dst);
    Item * arg_temp;
    Variable * v = dynamic_cast<Variable *>(element->arg);
    Number * n = dynamic_cast<Number*>(element->arg);
    if (v) {
      arg_temp = v;
    } else if (n) {
      arg_temp = n;
    }

    outputFile << "\t" << dst_temp->toString() << " <- call allocate(" << arg_temp->toString() << ", 1)" << std::endl;
  }
  void IR_Visitors::VisitInstruction(Instruction_label *element) {
    return;
  }
  void IR_Visitors::VisitInstruction(te_br_label *element) {
    auto lab = dynamic_cast<Label *>(element->label);
    outputFile << "\t" << "br " << lab->toString() << std::endl;
  }
  void IR_Visitors::VisitInstruction(te_br_t *element) {
    Item * temp; 
    Variable * v = dynamic_cast<Variable *>(element->t);
    Number * n = dynamic_cast<Number*>(element->t);
    if (v) {
      temp = v;
    } else if (n) {
      temp = n;
    }

    auto lab1 = dynamic_cast<Label *>(element->label1);
    auto lab2 = dynamic_cast<Label *>(element->label2);

    outputFile << "\t" << "br " << temp->toString() << " " << lab1->toString() << std::endl;
    outputFile << "\t" << "br " << lab2->toString() << std::endl;
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

      // if (f->arguments.size()) { // didn't go through other items yet so this doesn't work
      //   while(!f->arguments.empty()) {
      //     std::cout << std::to_string(f->arguments.size()) << std::endl;
      //     auto v_temp = f->arguments.back();
      //     outputFile << v_temp->toString();
      //     f->arguments.pop_back();
      //   }
      // }

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