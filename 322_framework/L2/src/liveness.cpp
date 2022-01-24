#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>

// included libraries
#include <unordered_set>

using namespace std;

namespace L2 {
  //string generate_liveness_list(Function f) {}

  //different def for each instruction
  //return gen item, kill item
  



  void create_liveness_file(Function f) {

    /*
     * Open output file
     */
    std::ofstream outputFile;
    outputFile.open("liveness.out");

    //* =========================== START OF LIVENESS ANALYSIS ===========================

    int numInstructions = f.instructions.size();
    
    //? what type should the elements of the unordered_set be? i think Item is okay
    std::vector<std::unordered_set<Item>> GEN[numInstructions]; //* GEN[i] = all variables read by instruction i
    std::vector<std::unordered_set<Item>> KILL[numInstructions]; //* KILL[i] = all variables written/defined by instruction i
    std::vector<std::unordered_set<Item>> IN[numInstructions];
    std::vector<std::unordered_set<Item>> OUT[numInstructions];

    /*
     * gen/kill creating vectors
     */

    // instructions to implement
    // 

    for (int i = 0; i < numInstructions; i++) {
      std::unordered_set<Item> gen_temp = {};
      std::unordered_set<Item> kill_temp = {};
      //check for dst/src args
      //if register or variable,
      //send signal, 1,2,3 , for each instruction add gen and kill, gen, kill
      //set a flag to say save x,y or just x or just y
      //rdi++ 

      //lea: w @ w w E
      //local values

      //if (f.instruction[i].dst) {
      //check if register or variable, gen[i].insert

      //check itype

      switch (f.instructions[i]->id) {
        case ret:
        {
          gen_temp = callee_save_set;
          gen_temp.insert(reg_ax);

          kill_temp = {};
        }
        case assignment:
        {
          if (f.instructions[i]->src.isVar) gen_temp.insert(f.instructions[i]->src);
          if (f.instructions[i]->dst.isVar) kill_temp.insert(f.instructions[i]->dst);
        }
        case arithmetic:
        {
          gen_temp.insert(f.instructions[i]->dst);
          kill_temp.insert(f.instructions[i]->dst);
          if (f.instructions[i]->src.isVar) gen_temp.insert(f.instructions[i]->src);
        }
        case crement:
        {
          gen_temp.insert(f.instructions[i]->dst);
          kill_temp.insert(f.instructions[i]->dst);
        }
        case shift:
        {
          gen_temp.insert(f.instructions[i]->dst);
          kill_temp.insert(f.instructions[i]->dst);
          if (f.instructions[i]->src.isVar) gen_temp.insert(f.instructions[i]->src);
        }
        case cmp:
        {
          kill_temp.insert(f.instructions[i].dst);
          if (f.instructions[i]->arg1.isVar) gen_temp.insert(f.instructions[i].arg1);
          if (f.instructions[i]->arg2.isVar) gen_temp.insert(f.instructions[i].arg2);
        }
        case cjump:
        {
          if (f.instructions[i].arg1.isVar) gen_temp.insert(f.instructions[i].arg1);
          if (f.instructions[i].arg2.isVar) gen_temp.insert(f.instructions[i].arg2);
        }
        case lea:
        {
          kill_temp.insert(f.instructions[i].dst);
          if (f.instructions[i].arg1.isVar) gen_temp.insert(f.instructions[i].arg1);
          if (f.instructions[i].arg2.isVar) gen_temp.insert(f.instructions[i].arg2);
        }
        case calls:
        {
          // int numArgs = stoi(f.instructions[i].N->offset);
          // gen_temp = arg_registers[numArgs];
          if (f.instructions[i].u->isVar) gen_temp.insert(f.instructions[i].u);

          kill_temp = caller_save_set{};
          //? more than six args?
        }
        case runtime:
        {
          // int numArgs = stoi(f.instructions[i].N.offset);
          // gen_temp = arg_registers[numArgs];
          kill_temp = caller_save_set{};
        }
        default: //gotoo, _label don't have anything in gen or kill sets
        {
          break
        }
      }

      cout << gen_temp << endl;
      cout << kill_temp << endl;
      
      GEN[i] = gen_temp;
      KILL[i] = kill_temp;
    }

    for (int i = 0; i < numInstructions; i++) {
      IN[i] = {}; // IN[i] = {};
      OUT[i] = {}; // OUT[i] = {};
    }

    // do {
    //   for (int i = 0; i < numInstructions; i++) {
    //     result
    //   }
    // }

    //* =========================== END OF LIVENESS ANALYSIS ===========================

    // Successors of an instruciton
    
    /*
     *close output file
     */
    outputFile.close();
    return ;
  }
}