#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// #include <L2.h>
#include <code_generator.h>
#include <L2_parser.h>
#include <liveness.h>
#include <interference.h>
#include <graph_coloring.h>
#include <spiller.h>

// included libraries
#include <unordered_set>
#include <tuple>

extern bool shouldPrint;

namespace L2 {
  std::map<std::string, int> color_to_reg = {{"red", reg::r10}, {"orange", reg::r11}, {"yellow", reg::r8}, {"green", reg::r9}, {"blue", reg::rax}, {"indigo", reg::rcx}, {"purple", reg::rdi}, {"pink", reg::rdx}, {"brown", reg::rsi}, {"black", reg::r12}, {"white", reg::r13}, {"gray", reg::r14}, {"maroon", reg::r15}, {"navy", reg::rbp}, {"periwinkle", reg::rbx}};

  void Gen_Code_Visitors::VisitInstruction (Instruction_ret *element) {
    this->outputFile << "\t\t" << "return" << std::endl;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_assignment *element) {
    if (shouldPrint) std::cout << "visited assignment instruction" << "\n";
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    std::string dst_str, src_str;

    // dst - w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    }
    else if (dynamic_cast<Register *>(dst) != nullptr) {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    } else if (dynamic_cast<Memory *>(dst) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr){
        auto temp_var = dynamic_cast<Variable*>(rv_temp);
        auto var_name = temp_var->toString();
        std::string col = this->color_graph->g->lookupNode(var_name)->color;
        int regi = color_to_reg[col];
        auto num_temp = std::get<1>(mem_fields);
        std::cout << "Before seg" << std::endl;
        num_temp = dynamic_cast<Number *>(num_temp);
        dst_str = "mem " + get_enum_string(regi) + " " + num_temp->toString();
        std::cout << "after seg" << std::endl;
      } else {
        // auto mem_temp = dynamic_cast<Memory *>(dst);
        dst_str = mem_temp->toString(); 
      }
    }

    // src - s
    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(src);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      src_str = get_enum_string(regi);
    } else if (dynamic_cast<Label *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Label *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Register *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Register *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Number *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Number *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Memory *>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr){
        auto temp_var = dynamic_cast<Variable*>(rv_temp);
        auto var_name = temp_var->toString();
        std::string col = this->color_graph->g->lookupNode(var_name)->color;
        int regi = color_to_reg[col];
        auto num_temp = std::get<1>(mem_fields);
        std::cout << "Before seg" << std::endl;
        num_temp = dynamic_cast<Number *>(num_temp);
        src_str = "mem " + get_enum_string(regi) + " " + num_temp->toString();
        std::cout << "after seg" << std::endl;
      } else {
        // auto mem_temp = dynamic_cast<Memory *>(src);
        src_str = mem_temp->toString(); 
      }
    }

    this->outputFile << "\t\t" + dst_str + " <- " + src_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to assignment instruction" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_arithmetic *element) {
    if (shouldPrint) std::cout << "visited Instruction_arithmetic" << "\n";
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    auto op = std::get<2>(fields);
    std::string dst_str, op_str, src_str;

    // dst - w + mem
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    }else if (dynamic_cast<Register *>(dst) != nullptr) {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    } else if (dynamic_cast<Memory *>(dst) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(dst);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr){
        auto temp_var = dynamic_cast<Variable*>(rv_temp);
        auto var_name = temp_var->toString();
        std::string col = this->color_graph->g->lookupNode(var_name)->color;
        int regi = color_to_reg[col];
        auto num_temp = std::get<1>(mem_fields);
        std::cout << "Before seg" << std::endl;
        num_temp = dynamic_cast<Number *>(num_temp);
        dst_str = "mem " + get_enum_string(regi) + " " + num_temp->toString();
        std::cout << "after seg" << std::endl;
      } else {
        auto mem_temp = dynamic_cast<Memory *>(dst);
        dst_str = mem_temp->toString(); 
      }
    }

    // op - op
    auto op_temp = dynamic_cast<Operation *>(op);
    op_str = op_temp->toString();

    // src - t + mem
    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(src);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      src_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Register *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Number *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Number *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Memory *>(src) != nullptr) {
      auto mem_temp = dynamic_cast<Memory *>(src);
      auto mem_fields = mem_temp->get();
      auto rv_temp = std::get<0>(mem_fields);
      if (dynamic_cast<Variable*>(rv_temp) != nullptr){
        auto temp_var = dynamic_cast<Variable*>(rv_temp);
        auto var_name = temp_var->toString();
        std::string col = this->color_graph->g->lookupNode(var_name)->color;
        int regi = color_to_reg[col];
        auto num_temp = std::get<1>(mem_fields);
        std::cout << "Before seg" << std::endl;
        num_temp = dynamic_cast<Number *>(num_temp);
        src_str = "mem " + get_enum_string(regi) + " " + num_temp->toString();
        std::cout << "after seg" << std::endl;
      } else {
        // auto mem_temp = dynamic_cast<Memory *>(src);
        src_str = mem_temp->toString(); 
      }
    }

    this->outputFile << "\t\t" + dst_str + " " + op_str + " " + src_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_arithmetic" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_crement *element) {
    if (shouldPrint) std::cout << "visited Instruction_crement" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto op = std::get<1>(fields);
    std::string dst_str, op_str;

    // dst - w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    }
    else {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    }

    // op - op
    auto op_temp = dynamic_cast<Operation *>(op);
    op_str = op_temp->toString();

    this->outputFile << "\t\t" + dst_str + op_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_crement" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_shift *element) {
    if (shouldPrint) std::cout << "visited Instruction_shift" << "\n";
    auto fields = element->get();
    auto src = std::get<0>(fields);
    auto dst = std::get<1>(fields);
    auto sop = std::get<2>(fields);
    std::string dst_str, sop_str, src_str;

    // dst - w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    }
    else {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    }

    // op - sop
    auto sop_temp = dynamic_cast<Operation *>(sop);
    sop_str = sop_temp->toString();

    // src - sx
    if (dynamic_cast<Variable*>(src) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(src);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      src_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Register *>(src);
      src_str = src_temp->toString();
    } else if (dynamic_cast<Number *>(src) != nullptr) {
      auto src_temp = dynamic_cast<Number *>(src);
      src_str = src_temp->toString();
    }

    this->outputFile << "\t\t" + dst_str + " " + sop_str + " " + src_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_shift" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_cmp *element) {
    if (shouldPrint) std::cout << "visited Instruction_cmp" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto a1 = std::get<1>(fields);
    auto a2 = std::get<2>(fields);
    auto op = std::get<3>(fields);
    std::string dst_str, arg1_str, cmp_str, arg2_str;

    // dst - w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    }
    else {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    }

    // arg1 - t
    if (dynamic_cast<Variable*>(a1) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a1);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg1_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(a1) != nullptr) {
      auto arg1_temp = dynamic_cast<Register *>(a1);
      arg1_str = arg1_temp->toString();
    } else if (dynamic_cast<Number *>(a1) != nullptr) {
      auto arg1_temp = dynamic_cast<Number *>(a1);
      arg1_str = arg1_temp->toString();
    }

    // op - cmp
    auto cmp_temp = dynamic_cast<Operation *>(op);
    cmp_str = cmp_temp->toString();

    // arg2 - t
    if (dynamic_cast<Variable*>(a2) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a2);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg2_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(a2) != nullptr) {
      auto arg2_temp = dynamic_cast<Register *>(a2);
      arg2_str = arg2_temp->toString();
    } else if (dynamic_cast<Number *>(a2) != nullptr) {
      auto arg2_temp = dynamic_cast<Number *>(a2);
      arg2_str = arg2_temp->toString();
    }
      
    this->outputFile << "\t\t" + dst_str + " <- " + arg1_str + " " + cmp_str + " " + arg2_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_cmp" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_cjump *element) {
    if (shouldPrint) std::cout << "visited Instruction_cjump" << "\n";
    auto fields = element->get();
    auto a1 = std::get<0>(fields);
    auto a2 = std::get<1>(fields);
    auto lab = std::get<2>(fields);
    auto op = std::get<3>(fields);
    std::string arg1_str, cmp_str, arg2_str, lab_str;

    // arg1 - t
    if (dynamic_cast<Variable*>(a1) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a1);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg1_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(a1) != nullptr) {
      auto arg1_temp = dynamic_cast<Register *>(a1);
      arg1_str = arg1_temp->toString();
    } else if (dynamic_cast<Number *>(a1) != nullptr) {
      auto arg1_temp = dynamic_cast<Number *>(a1);
      arg1_str = arg1_temp->toString();
    }

    // op - cmp
    auto cmp_temp = dynamic_cast<Operation *>(op);
    cmp_str = cmp_temp->toString();

    // arg2 - t
    if (dynamic_cast<Variable*>(a2) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a2);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg2_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(a2) != nullptr) {
      auto arg2_temp = dynamic_cast<Register *>(a2);
      arg2_str = arg2_temp->toString();
    } else if (dynamic_cast<Number *>(a2) != nullptr) {
      auto arg2_temp = dynamic_cast<Number *>(a2);
      arg2_str = arg2_temp->toString();
    }

    // lab - label
    auto lab_temp = dynamic_cast<Label *>(lab);
    lab_str = lab_temp->toString();

    this->outputFile << "\t\t" << "cjump " + arg1_str + " " + cmp_str + " " + arg2_str + " " + lab_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_cjump" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_lea *element) {
    if (shouldPrint) std::cout << "visited Instruction_lea" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto a1 = std::get<1>(fields);
    auto a2 = std::get<2>(fields);
    auto mul = std::get<3>(fields);
    std::string dst_str, arg1_str, arg2_str, mul_str;

    // dst - w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    } else {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    }

    // arg1 - w
    if (dynamic_cast<Variable*>(a1) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a1);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg1_str = get_enum_string(regi);
    } else {
      auto a1_temp = dynamic_cast<Register *>(a1);
      arg1_str = a1_temp->toString();
    }

    // arg2 - w
    if (dynamic_cast<Variable*>(a2) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(a2);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      arg2_str = get_enum_string(regi);
    } else {
      auto a2_temp = dynamic_cast<Register *>(a2);
      arg2_str = a2_temp->toString();
    }

    // mul - num
    auto mul_temp = dynamic_cast<Number *>(mul);
    mul_str = mul_temp->toString();

    this->outputFile << "\t\t" + dst_str + " @ " + arg1_str + " " + arg2_str + " " + mul_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_lea" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_calls *element) {
    if (shouldPrint) std::cout << "visited Instruction_calls" << "\n";
    auto fields = element->get();
    auto u = std::get<0>(fields);
    auto N = std::get<1>(fields);
    std::string u_str, N_str;

    // u - w and label
    if (dynamic_cast<Variable*>(u) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(u);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      u_str = get_enum_string(regi);
    } else if (dynamic_cast<Register *>(u) != nullptr) {
      auto u_temp = dynamic_cast<Register *>(u);
      u_str = u_temp->toString();
      } else if (dynamic_cast<Label *>(u) != nullptr) {
      auto u_temp = dynamic_cast<Label *>(u);
      u_str = u_temp->toString();
      }

    // if variable pointer object, dynamic cast and get() -> return string
    // use string in lookUp Node method, find the Node*
    // from Node* -> get color from field
    // use color as key in unordered map color_to_reg -> returns reg
    // use get_enum_string() to get register string

    // N - number
    auto N_temp = dynamic_cast<Number *>(N);
    N_str = N_temp->toString();

    this->outputFile << "\t\t" << "call " + u_str + " " + N_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_calls" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_runtime *element) {
    this->outputFile << "\t\t" << element->toString() << std::endl;
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_label *element) {
    if (shouldPrint) std::cout << "visited Instruction_label" << "\n";
    auto fields = element->get();
    auto lab = std::get<0>(fields);
    std::string lab_str;

    // lab - label
    auto lab_temp = dynamic_cast<Label *>(lab);
    lab_str = lab_temp->toString();

    this->outputFile << "\t\t" + lab_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_label" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_goto *element) {
    if (shouldPrint) std::cout << "visited Instruction_goto" << "\n";
    auto fields = element->get();
    auto lab = std::get<0>(fields);
    std::string lab_str;

    // lab - label
    auto lab_temp = dynamic_cast<Label *>(lab);
    lab_str = lab_temp->toString();

    this->outputFile << "\t\t" << "goto " + lab_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_goto" << "\n";
  }
  void Gen_Code_Visitors::VisitInstruction (Instruction_stackarg *element) {
    if (shouldPrint) std::cout << "visited Instruction_stackarg" << "\n";
    auto fields = element->get();
    auto dst = std::get<0>(fields);
    auto M = std::get<1>(fields);
    std::string dst_str, M_str;

    // w
    if (dynamic_cast<Variable*>(dst) != nullptr) {
      auto temp_var = dynamic_cast<Variable*>(dst);
      auto var_name = temp_var->toString();
      std::string col = this->color_graph->g->lookupNode(var_name)->color;
      int regi = color_to_reg[col];
      dst_str = get_enum_string(regi);
    } else {
      auto dst_temp = dynamic_cast<Register *>(dst);
      dst_str = dst_temp->toString();
    }

    // number
    auto M_temp = dynamic_cast<Number *>(M);
    int num_temp = M_temp->get();
    num_temp = this->f->num_locals * 8 + num_temp;
    M_str = std::to_string(num_temp);

    this->outputFile << "\t\t" << dst_str + " <- mem rsp " + M_str << std::endl;
    if (shouldPrint) std::cout << "ended visit to Instruction_stackarg" << "\n";
  }

  Gen_Code_Visitors::Gen_Code_Visitors(std::ofstream &outF, Function* f, ColorGraph* color_graph) : outputFile(outF), f(f), color_graph(color_graph) {
    
  }

  void generate_L1_file(Program p) {
    /* Create Output File */
    std::ofstream outputFile;
    outputFile.open("prog.L1");

    // L2::Program new_p;
    // new_p.entryPointLabel = p.entryPointLabel;

    /* Create Program Header */
    outputFile << "(" << p.entryPointLabel << std::endl;

    for (auto f : p.functions) {
      /* Register Allocation Loop */
      if (shouldPrint) std::cout << "start register allocation" << std::endl;
      bool mustSpill;
      Function* func_copy;
      func_copy = f;
      Graph* interference_graph;
      ColorGraph* color_graph; // set visitor's color_graph field here
      std::vector<Variable*> spill_list;
      std::string prefix = "%JACOBEUGENEAREAWESOME";
      int count = 0;
      do {
        //! make sure to check the edge case of can't color and can't spill
        std::cout << "iterate analysis" << std::endl;
        mustSpill = false;
        spill_list.clear();
        create_liveness_list(func_copy);
        std::cout << "created liveness list" << std::endl;
        interference_graph = create_interference_graph(func_copy);
        std::cout << "created interference graph" << std::endl;
        color_graph = registerAllocate(interference_graph);
        std::cout << "created color graph" << std::endl;
        std::cout << "variables to be spilled: ";
        for (auto var : color_graph->tobeSpilled) {
          std::string var_name = var->toString();
          if (var_name.compare(0, 22, prefix) != 0) {
            spill_list.push_back(var);
            // std::cout << var_name << " ";
          }
        }
        std::cout << std::endl;
        if (!spill_list.empty()) {
          mustSpill = true;
          std::cout << "spilling" << std::endl;
          func_copy = spill_mult_var(func_copy, color_graph->tobeSpilled, prefix);
          std::cout << "done spilling" << std::endl;
        }
      } while (mustSpill);
      
      /* Create function header */
      outputFile << "\t" << "(" << func_copy->name << std::endl;
      outputFile << "\t\t" << func_copy->arguments << " " << func_copy->num_locals << std::endl;

      /* Call visitor pattern */
      Gen_Code_Visitors v {outputFile, func_copy, color_graph};

      for (auto i : func_copy->instructions) {
        i->Accept(&v);
      }

      /* Create function tail */
      outputFile << "\t" << ")" << std::endl;
    }

    /* Create Program tail */
    outputFile << ")" << std::endl;
  }
}