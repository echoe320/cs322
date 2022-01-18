#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

string eight_bit_variants(string reg){
  string value;
  if (reg == "r10"||"r11"||"r12"||"r13"||"r14"||"r15"||"r8"||"r9"){
    value = reg + "b"; 
  } else if (reg == "rax"){
    value = "%al";
  } else if (reg == "rbp"){
    value = "%pl";
  } else if (reg == "rbx"){
    value = "%bl";
  }

  return value;
}

namespace L1{
  string ret_arg(Item arg, bool eight_bit){
    string value;
    if (eight_bit){
      value = eight_bit_variants(arg.Register);
    } else {
      if (arg.isARegister) {
        value = "%" + arg.Register;
      } else if (arg.isMem) {
        value = arg.offset +  "(%" + arg.Register + ")";
      } else if (arg.isNum) {
        value = "$" + arg.offset;
      } else if (arg.isLabel) {
        value = arg.labelName;
      }
    }

    return value;
  }

  string ret_op(operation arg){
    string value;
    if (arg.op == "+=") value = "addq";
    if (arg.op == "-=") value = "subq";
    if (arg.op == "*=") value = "imulq";
    if (arg.op == "&=") value = "andq";
    if (arg.op == "++") value = "inc";
    if (arg.op == "--") value = "dec";
    if (arg.op == ">>=") value = "sarq";
    if (arg.op == "<<=") value = "salq";

    return value;
  }

  void generate_code(Program p){

    /* 
     * Open the output file.
     */ 
    std::ofstream outputFile;
    outputFile.open("prog.S");


    //Create initial file
    outputFile << ".text\n";
    outputFile << "\t.globl go\n";
    outputFile << "go:\n";

    //save callee_saved registers

    string callee_reg[6] = {"%rbx", "%rbp", "%r12", "%r13", "%r14", "%r15"};
    for (int i = 0; i < 5; i++) {
      outputFile << "\tpushq " << callee_reg[i] << "\n";
    }
    
    //main function call
    outputFile << "\tcall " << "_" + p.entryPointLabel.substr(1)<< "\n";

    //restore callee_saved registers

    for (int i = 5; i > 0; i++) {
      outputFile << "\tpopq " << callee_reg[i] << "\n";
    }

    outputFile << "\tretq\n";
    
    //functions

    string double_tab = "\t\t";
    string new_line = "\n";

    for (auto func : p.functions) {
      outputFile << "\t_" << func->name << "\n";
      for (auto i : func->instructions) {
        string instruction = double_tab;
        switch (i->id) {
          case ret:
          {
            instruction += "retq";
            break;
          }
          case assignment:
          {
            Instruction_assignment *inst = static_cast<Instruction_assignment *>(i);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, false);
            instruction += "movq " + src + ", " + dst;
            break;
          }
          case arithmetic:
          {
            Instruction_arithmetic *inst = static_cast<Instruction_arithmetic *>(i);
            string op_arg = ret_op(inst->op);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, false);
            instruction += op_arg + " " + src + ", " + dst;
            break;
          }
          case crement:
          {
            Instruction_crement *inst = static_cast<Instruction_crement*>(i);
            string op_arg = ret_op(inst->op);
            string dst = ret_arg(inst->dst, false);
            instruction += op_arg + " " + dst;
            break;
          }
          case shift:
          {
            Instruction_shift *inst = static_cast<Instruction_shift*>(i);
            string op_arg = ret_op(inst->op);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, false);
            instruction += op_arg + " " + src + ", " + dst;
            break;
          }
          case cmp:
          {
            break;
          }
          case cjump:
          {
            break;
          }
          case lea:
          {
            break;
          }
          case calls:
          {
            break;
          }
          case _label:
          {
            break;
          }
          case gotoo:
          {
            break;
          }
          default:
            break;
        }
        outputFile << instruction + new_line;
        //auto inst_type = type(i)
        //case(Instruction_assignment *a = dynamic_cast<Instruction_assignment *>(i))
        // switch(inst_type) {
        //   case constant-expression  :
        //     statement(s);
        //     break; //optional
        //   case constant-expression  :
        //       statement(s);
        //       break; //optional
          
          // you can have any number of case statements.
//           default : //Optional
//               statement(s);
        // }
    
      }
      outputFile << "" << "\n";

     outputFile << "\tretq\n";
    }
    
    //outputFile << 
   
    /* 
     * Generate target code
     */ 
    //TODO

    //program p
    
    //check p.entryPointLabel

    //p.entryPointLabel;

    //p.

    //case (a):
      //if ("<<=") 

    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return ;
  }
}
