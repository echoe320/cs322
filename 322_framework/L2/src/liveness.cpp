#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>
//#include <L2.h>

// included libraries
#include <unordered_set>

using namespace std;

namespace L2 {
  //string generate_liveness_list(Function f) {}

  //different def for each instruction
  //return gen item, kill item
  

  void create_liveness_file(Function f) {
    /*
     * Create variables
     */
    //Callee save register items
    Item reg_12, reg_13, reg_14, reg_15, reg_bp, reg_bx;
    // for (auto i: ) {

    // }
    reg_12.r = r12; 
    reg_13.r = r13; 
    reg_14.r = r14; 
    reg_15.r = r15; 
    reg_bp.r = rbp; 
    reg_bx.r = rbx;

    //Caller save register items
    Item reg_10, reg_11, reg_8, reg_9, reg_ax, reg_cx, reg_di, reg_dx, reg_si;
    reg_10.r = r10; 
    reg_11.r = r11; 
    reg_8.r = r8; 
    reg_9.r = r9; 
    reg_ax.r = rax; 
    reg_cx.r = rcx; 
    reg_di.r = rdi; 
    reg_dx.r = rdx; 
    reg_si.r = rsi;

    class reg_var {
      enum reg {regs, var}; //num -> string
      string v; // reg string?
    }

    //Callee and caller sets
    unordered_set<Item> callee_save_set = {reg_12, reg_13, reg_14, reg_15, reg_bp, reg_bx};
    unordered_set<Item> caller_save_set = {reg_10, reg_11, reg_8, reg_9, reg_ax, reg_cx, reg_di, reg_dx, reg_si};

    //Arg Register set
    //std::vector<std::string> arg_registers[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax"};
    vector<unordered_set<Item>> arg_registers(7);
    

    /*
     * Open output file
     */
    std::ofstream outputFile;
    outputFile.open("liveness.out");

    //* =========================== START OF LIVENESS ANALYSIS ===========================

    int numInstructions = f.instructions.size();
    
    //? what type should the elements of the unordered_set be? i think Item is okay
    vector<unordered_set<Item>> GEN; //[numInstructions]; //* GEN[i] = all variables read by instruction i
    vector<unordered_set<Item>> KILL; //[numInstructions]; //* KILL[i] = all variables written/defined by instruction i
    vector<unordered_set<Item>> IN; //[numInstructions];
    vector<unordered_set<Item>> OUT; //[numInstructions];

    /*
     * gen/kill creating vectors
     */

    // instructions to implement
    // 

    //for (int i = 0; i < numInstructions; i++) {

    //int count = 0;
    
    for (auto i : f.instructions) {
      unordered_set<Item> gen_temp;
      unordered_set<Item> kill_temp;
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

      switch (i->id) {
        case ret:
        {
          gen_temp = callee_save_set;
          gen_temp.insert(reg_ax);

          kill_temp = {};
          break;
        }
        case assignment:
        {
          Instruction_assignment *inst = static_cast<Instruction_assignment *>(i);
          if (inst->src.isVar) gen_temp.insert(inst->src);
          if (inst->dst.isVar) kill_temp.insert(inst->dst);
          break;
        }
        case arithmetic:
        {
          Instruction_arithmetic *inst = static_cast<Instruction_arithmetic *>(i);
          gen_temp.insert(inst->dst);
          kill_temp.insert(inst->dst);
          if (inst->src.isVar) gen_temp.insert(inst->src);
          break;
        }
        case crement:
        {
          Instruction_crement *inst = static_cast<Instruction_crement*>(i);
          gen_temp.insert(inst->dst);
          kill_temp.insert(inst->dst);
          break;
        }
        case shift:
        {
          Instruction_shift *inst = static_cast<Instruction_shift*>(i);
          gen_temp.insert(inst->dst);
          kill_temp.insert(inst->dst);
          if (inst->src.isVar) gen_temp.insert(inst->src);
          break;
        }
        case cmp:
        {
          Instruction_cmp *inst = static_cast<Instruction_cmp*>(i);
          kill_temp.insert(inst->dst);
          if (inst->arg1.isVar) gen_temp.insert(inst->arg1);
          if (inst->arg2.isVar) gen_temp.insert(inst->arg2);
          break;
        }
        case cjump:
        {
          Instruction_cjump *inst = static_cast<Instruction_cjump*>(i);
          if (inst->arg1.isVar) gen_temp.insert(inst->arg1);
          if (inst->arg2.isVar) gen_temp.insert(inst->arg2);
          break;
        }
        case lea:
        {
          Instruction_lea *inst = static_cast<Instruction_lea*>(i);
          kill_temp.insert(inst->dst);
          if (inst->arg1.isVar) gen_temp.insert(inst->arg1);
          if (inst->arg2.isVar) gen_temp.insert(inst->arg2);
          break;
        }
        case calls:
        {
          // int numArgs = stoi(f.instructions[i].N->offset);
          // gen_temp = arg_registers[numArgs];
          Instruction_calls *inst = static_cast<Instruction_calls*>(i);
          if (inst->u.isVar) gen_temp.insert(inst->u);
          kill_temp = caller_save_set;
          //? more than six args?
          break;
        }
        case runtime:
        {
          Instruction_runtime *inst = static_cast<Instruction_runtime*>(i);
          // int numArgs = stoi(f.instructions[i].N.offset);
          // gen_temp = arg_registers[numArgs];
          kill_temp = caller_save_set;
          break;
        }
        default: //gotoo, _label don't have anything in gen or kill sets
        {
          break;
        }
      }

      // for (auto i : gen_temp) {
      //   cout <<  << endl;
      // }
      
      // cout << to_string(kill_temp) << endl;
      
      GEN.push_back(gen_temp);
      KILL.push_back(kill_temp);
      //count++;
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