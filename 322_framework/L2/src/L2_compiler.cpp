#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

#include <L2.h>
#include <liveness.h>
#include <interference.h>
#include <spiller.h>
#include <graph_coloring.h>
#include <L2_parser.h>
#include <code_generator.h>

using namespace std;

void print_help (char *progName){
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] [-s] [-l] [-i] SOURCE" << std::endl;
  return ;
}

int main(
  int argc, 
  char **argv
  ){
  // std::cout << "STARTING FROM THE VERY BEGINNING" << std::endl;
  auto enable_code_generator = true;
  auto spill_only = false;
  auto interference_only = false;
  auto liveness_only = false;
  int32_t optLevel = 3;

  /* 
   * Check the compiler arguments.
   */
  auto verbose = false;
  if( argc < 2 ) {
    print_help(argv[0]);
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "vg:O:sli")) != -1) {
    switch (opt){

      case 'l':
        liveness_only = true;
        break ;

      case 'i':
        interference_only = true;
        break ;

      case 's':
        spill_only = true;
        break ;

      case 'O':
        optLevel = strtoul(optarg, NULL, 0);
        break ;

      case 'g':
        enable_code_generator = (strtoul(optarg, NULL, 0) == 0) ? false : true ;
        break ;

      case 'v':
        verbose = true;
        break ;

      default:
        print_help(argv[0]);
        return 1;
    }
  }

  /*
   * Parse the input file.
   */
  L2::Program p;
  // L2::Function f;
  //auto p,f;
  if (spill_only){

    /* 
     * Parse an L2 function and the spill arguments.
     */
    p = L2::parse_spill_file(argv[optind]);
 
  } else if (liveness_only){

    /*
     * Parse an L2 function.
     */
    // std::cout << "Starting Parsing file now" << std::endl;
    p = L2::parse_function_file(argv[optind]);
    // std::cout << "Ending Parsing file now" << std::endl;

  } else if (interference_only){

    /*
     * Parse an L2 function.
     */
    p = L2::parse_function_file(argv[optind]);

  } else {

    /* 
     * Parse the L2 program.
     */
    p = L2::parse_file(argv[optind]);
  }

  /*
   * Special cases.
   */
  if (spill_only){
    auto func = L2::spill_mult_var(p.functions[0], p.toSpill, p.prefix);
    // have to print func separately
    std::cout << "(" << func->name << "\n\t";
    std::cout << std::to_string(func->arguments) << " " << std::to_string(func->num_locals) << "\n";
    
    for (auto i : func->instructions) {
      std::cout << "\t" << i->toString() << "\n";
    }

    std::cout << ")" << std::endl;
    return 0;
  }

  /*
   * Liveness test.
   */
  if (liveness_only){
    for (auto f : p.functions) {
      L2::create_liveness_list(f);
      f->printINOUTsets();
    }
    return 0;
  }

  /*
   * Interference graph test.
   */
  if (interference_only){
    for (auto f : p.functions) {
      L2::create_liveness_list(f);
      auto graph = L2::create_interference_graph(f);
      for (auto it = graph->g.begin(); it != graph->g.end(); ++it) {
        auto temp = *it;
        std::cout << temp.first->name << " ";
        for (auto item : temp.second) {
          std::cout << item->name << " ";
        }
        std::cout << std::endl;
      }
      auto colorGraph = L2::registerAllocate(graph);
    }
    // for (auto f : p.functions) f->printInterferenceGraph();
    return 0;
  }

  /*
   * Generate the target code.
   */
  if (enable_code_generator){
    // L2::Program new_p;
    // new_p.entryPointLabel = p.entryPointLabel;
    // L2::create_liveness_list(p);

    // // register allocation loop
    // for (auto f : p.functions) {
    //   bool mustSpill;
    //   L2::Function* func_copy = f;
    //   L2::Graph* interference_graph;
    //   L2::ColorGraph* color_graph;
    //   do {
    //     //! make sure to check the edge case of can't color and can't spill
    //     mustSpill = false;
    //     interference_graph = L2::create_interference_graph(func_copy);
    //     color_graph = L2::registerAllocate(interference_graph);
    //     if (!color_graph->tobeSpilled.empty()) {
    //       mustSpill = true;
    //       func_copy = L2::spill_mult_var(func_copy, color_graph->tobeSpilled, "%JACOBEUGENEAREAWESOME");
    //     }
    //   } while (!mustSpill);
    //   L2::Function* new_func = func_copy;
    //   new_func->instructions.clear();
    //   for (auto inst : func_copy->instructions) {
    //     if (dynamic_cast<L2::Instruction_ret*>(inst) != nullptr) {
    //       auto temp = dynamic_cast<L2::Instruction_ret*>(inst);
    //       new_func->instructions.push_back(temp);
    //     }
    //     else if (dynamic_cast<L2::Instruction_assignment*>(inst) != nullptr) {
    //       //! revisit this one
    //       auto temp = dynamic_cast<L2::Instruction_assignment*>(inst);
    //       auto fields = temp->get();
    //       auto src = std::get<0>(fields);
    //       auto dst = std::get<1>(fields);
    //       L2::Item* new_src;
    //       L2::Item* new_dst;

    //       if (dynamic_cast<L2::Memory*>(src) != nullptr) {
    //         auto mem_temp = dynamic_cast<L2::Memory*>(src);
    //         auto mem_fields = mem_temp->get();
    //         auto rv_temp = std::get<0>(mem_fields);
    //         auto offset_temp = std::get<1>(mem_fields);
    //         if (dynamic_cast<L2::Variable*>(rv_temp) != nullptr) {
    //           // L2::Register* new_rv = new Register(access colormap);
    //           continue;
    //         }
    //         // L2::Memory* new_mem = new Memory(new_rv, offset_temp);
    //         continue;
    //       }
    //       if (dynamic_cast<L2::Memory*>(dst) != nullptr) {
    //         auto mem_temp = dynamic_cast<L2::Memory*>(dst);
    //         auto mem_fields = mem_temp->get();
    //         auto rv_temp = std::get<0>(mem_fields);
    //         auto offset_temp = std::get<1>(mem_fields);
    //         if (dynamic_cast<L2::Variable*>(rv_temp) != nullptr) {
    //           // L2::Register* new_rv = new Register(access colormap);
    //           continue;
    //         }
    //         // L2::Memory* new_mem = new Memory(new_rv, offset_temp);
    //         continue;
    //       }
    //       if (dynamic_cast<L2::Variable*>(src) != nullptr) {
    //         auto var_temp = dynamic_cast<L2::Variable*>(src);
    //         // L2::Register* new_reg = new Register(access colormap);
    //       }
    //       if (dynamic_cast<L2::Variable*>(src) != nullptr) {
    //         auto var_temp = dynamic_cast<L2::Variable*>(src);
    //         // L2::Register* new_reg = new Register(access colormap);
    //       }
    //     }
    //     else if (dynamic_cast<L2::Instruction_arithmetic*>(inst) != nullptr) {
    //       //! revisit this one
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_crement*>(inst) != nullptr) {
    //       auto temp = dynamic_cast<L2::Instruction_crement*>(inst);
    //       auto fields = temp->get();
    //       auto dst = std::get<0>(fields);
    //       auto op = std::get<1>(fields);
    //       L2::Register* new_dst = dst;

    //       if (dynamic_cast<L2::Variable*>(dst) != nullptr) {
    //         std::string color = color_graph->g->lookupNode(dst->toString())->color;
    //         new_dst = new L2::Register(static_cast<L2::reg>(L2::color_to_reg[color]));
    //       }
    //       L2::Instruction_crement* new_inst = new L2::Instruction_crement(new_dst, op);
    //       new_func->instructions.push_back(new_inst);
    //     }
    //     else if (dynamic_cast<L2::Instruction_shift*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_cmp*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_cjump*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_lea*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_calls*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_runtime*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_label*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_goto*>(inst) != nullptr) {
    //       continue;
    //     }
    //     else if (dynamic_cast<L2::Instruction_stackarg*>(inst) != nullptr) {
    //       continue;
    //     }
    //   }
    //   new_p.functions.push_back(new_func);
    // }
    L2::generate_L1_file(p);
  }

  return 0;
}
