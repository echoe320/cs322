#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace L1{
  string eight_bit_variants(string reg){
    string value;
    if (reg == "r10"|| reg == "r11"|| reg == "r12"|| reg == "r13"|| reg == "r14"|| reg == "r15"|| reg == "r8"|| reg == "r9"){
      value = reg + "b"; 
    } else if (reg == "rax"){
      value = "%al";
    } else if (reg == "rbp"){
      value = "%pl";
    } else if (reg == "rbx"){
      value = "%bl";
    } else if (reg == "rcx"){
      value = "%cl";
    } else if (reg == "rdi"){
      value = "%dil";
    } else if (reg == "rdx"){
      value = "%dl";
    } else if (reg == "rsi"){
      value = "%sil";
    } 

    return value;
  }

  string ret_arg(Item arg, bool eight_bit){
    string value;
    if (arg.isARegister) {
      if (eight_bit) {
        value = eight_bit_variants(arg.Register);
      } else {
        value = "%" + arg.Register;
      }
    } else if (arg.isMem) {
      value = arg.offset +  "(%" + arg.Register + ")";
    } else if (arg.isNum) {
      value = "$" + arg.offset;
    } else if (arg.isLabel) {
      value = "_" + arg.labelName.substr(1);
    }

    return value;
  }

  string ret_op(operation arg, bool iscjump){
    string value;
    if (arg.op == "+="){
      value = "addq";
    } else if (arg.op == "-=") {
      value = "subq";
    } else if (arg.op == "*=") {
      value = "imulq";
    } else if (arg.op == "&=") {
      value = "andq";
    } else if (arg.op == "++") {
      value = "inc";
    } else if (arg.op == "--") {
      value = "dec";
    } else if (arg.op == ">>=") {
      value = "sarq";
    } else if (arg.op == "<<=") {
      value = "salq";
    } else if (arg.op == "<=") {
      if (iscjump){
        value = "jle";
      } else {
        value = "setle";
      }
    } else if (arg.op == "<") {
      if (iscjump){
        value = "jl";
      } else {
        value = "setl";
      }
    } else if (arg.op == "=") {
      if (iscjump){
        value = "je";
      } else {
        value = "sete";
      }
    }

    return value;
  }

  string switch_cmp(string op) {
    string value;
    if (op == "setle"){
      value = "setge";
    } else if (op == "setl") {
      value = "setg";
    }
    return value;
  }

  string switch_cjump(string op) {
    string value;
    if (op == "jle"){
      value = "jge";
    } else if (op == "jl") {
      value = "jg";
    }
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
    for (int i = 0; i < 6; i++) {
      outputFile << "\tpushq " << callee_reg[i] << "\n";
    }
    
    //main function call
    outputFile << "\tcall " << "_" + p.entryPointLabel.substr(1)<< "\n";

    //restore callee_saved registers

    for (int i = 5; i > -1; i--) {
      outputFile << "\tpopq " << callee_reg[i] << "\n";
    }

    outputFile << "\tretq\n";
    
    //functions
    string single_tab = "\t";
    string double_tab = "\t\t";
    string new_line = "\n";

    for (auto func : p.functions) {
      outputFile << "_" << func->name.substr(1) + ":" << "\n";

      //extra args
      // if (func->arguments - 6 > 0){
        
      // }

      //locals
      int64_t num_locals = func->locals;
      //for (int64_t i = num_locals; i > 0; i--){
      string temp_num = '$' + to_string(num_locals*8);
      outputFile << "\tsubq " + temp_num + ", %rsp\n";
      //}

      for (auto i : func->instructions) {
        string instruction;
        switch (i->id) {
          case ret:
          {
            instruction += "addq " + temp_num + ", %rsp\n";
            instruction += single_tab + "retq";
            break;
          }
          case assignment:
          {
            Instruction_assignment *inst = static_cast<Instruction_assignment *>(i);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, false);
            if (inst->dst.isMem && inst->src.isLabel){
              src = "$" + src;
            }
            instruction += "movq " + src + ", " + dst;
            break;
          }
          case arithmetic:
          {
            Instruction_arithmetic *inst = static_cast<Instruction_arithmetic *>(i);
            string op_arg = ret_op(inst->op, false);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, false);
            instruction += op_arg + " " + src + ", " + dst;
            break;
          }
          case crement:
          {
            Instruction_crement *inst = static_cast<Instruction_crement*>(i);
            string op_arg = ret_op(inst->op, false);
            string dst = ret_arg(inst->dst, false);
            instruction += op_arg + " " + dst;
            break;
          }
          case shift:
          {
            Instruction_shift *inst = static_cast<Instruction_shift*>(i);
            string op_arg = ret_op(inst->op, false);
            string dst = ret_arg(inst->dst, false);
            string src = ret_arg(inst->src, true);
            instruction += op_arg + " " + src + ", " + dst;
            break;
          }
          case cmp:
          {
            Instruction_cmp *inst = static_cast<Instruction_cmp*>(i);
            string dst = ret_arg(inst->dst, false);
            string dst_eb = ret_arg(inst->dst, true);
            string arg1 = ret_arg(inst->arg1, false);
            string arg2 = ret_arg(inst->arg2, false);

            if (arg1.substr(0,1) == "$" && arg2.substr(0,1) == "$") { //case constant constant
              bool comp;
              if (inst->op.op == "<="){
                comp = (stoi(arg1.substr(1)) <= stoi(arg2.substr(1)));
              } else if (inst->op.op == "<"){
                comp = stoi(arg1) < stoi(arg2);
              } else if (inst->op.op == "="){
                comp = stoi(arg1) == stoi(arg2);
              }
              instruction += "movq $" + to_string(int(comp)) + ", " + dst;
              
            } else {
              string op_arg = ret_op(inst->op, false);
              if (arg1.substr(0,1) == "$") {
                string temp_arg = arg1;
                arg1 = arg2;
                arg2 = temp_arg;
                op_arg = switch_cmp(op_arg);
              }
              instruction += "cmpq " + arg2 + ", " + arg1 + new_line; // line 1
              instruction += single_tab + op_arg + " " + dst_eb + new_line; // line 2
              instruction += single_tab + "movzbq " + dst_eb + ", " + dst; // line 3
            }
            break;
          }
          case cjump:
          {
            Instruction_cjump *inst = static_cast<Instruction_cjump*>(i);
            string label = ret_arg(inst->label, false);
            string arg1 = ret_arg(inst->arg1, false);
            string arg2 = ret_arg(inst->arg2, false);

            if (arg1.substr(0,1) == "$" && arg2.substr(0,1) == "$") { //case constant constant
              bool comp;
              if (inst->op.op == "<="){
                comp = (stoi(arg1.substr(1)) <= stoi(arg2.substr(1)));
              } else if (inst->op.op == "<"){
                comp = stoi(arg1) < stoi(arg2);
              } else if (inst->op.op == "="){
                comp = stoi(arg1) == stoi(arg2);
              }
              if (comp == true) {
                instruction += "jmp " + label;
              }
              
            } else {
              string op_arg = ret_op(inst->op, true);
              if (arg1.substr(0,1) == "$") {
                string temp_arg = arg1;
                arg1 = arg2;
                arg2 = temp_arg;
                op_arg = switch_cjump(op_arg);
              }
              instruction += "cmpq " + arg2 + ", " + arg1 + new_line; // line 1
              instruction += single_tab + op_arg + " " + label; // line 2
            }
            break;
          }
          case lea:
          {
            Instruction_lea *inst = static_cast<Instruction_lea*>(i);
            string dst = ret_arg(inst->dst, false);
            string arg1 = ret_arg(inst->arg1, false);
            string arg2 = ret_arg(inst->arg2, false);
            string multiple = inst->multiple.offset;
            instruction += "lea (" + arg1 + ", " + arg2 + ", " + multiple + "), " + dst;
            break;
          }
          case calls:
          {
            Instruction_calls *inst = static_cast<Instruction_calls*>(i);
            string runtime_function = inst->u.labelName;
            int N = stoi(inst->N.offset);

            //runtime
            if (runtime_function == "print"){
              instruction += "call print";
            } else if (runtime_function == "allocate"){
              instruction += "call allocate";
            } else if (runtime_function == "input"){
              instruction += "call input";
            } else if (runtime_function == "tensor-error"){
              if (N == 1) {
                instruction += "call array_tensor_error_null";
              } else if (N == 3){
                instruction += "call array_error";
              } else if (N == 4){
                instruction += "call tensor-error";
              }
            } else {
              string u = ret_arg(inst->u, false);
              int space;
              //formula
              if (N>6) {
                space = (N-6)*8 + 8;
              } else {
                space = 8;
              }
              if (inst->u.isARegister == true) {
                u = "*" + u;
              }

              instruction +=  "subq $" + to_string(space) + ", %rsp\n";
              instruction +=  single_tab + "jmp " + u;
            }
            break;
          }
          case _label:
          {
            Instruction_label *inst = static_cast<Instruction_label*>(i);
            string arg_label = ret_arg(inst->label, false);
            instruction += arg_label + ":";
            break;
          }
          case gotoo:
          {
            Instruction_label *inst = static_cast<Instruction_label*>(i);
            string arg_label = ret_arg(inst->label, false);
            instruction += "jmp " + arg_label;
            break;
          }
          default:
            break;
        }
        //outputFile << instruction + new_line;
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
        // if (i->id == ret) {
        //   instruction = "\t" + instruction;
        // } else {
        //   instruction = double_tab + instruction;
        // }

        outputFile << "\t" << instruction + new_line;
      }
    //outputFile << "" << "\n";

     //outputFile << "\tretq\n";
    }

    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return ;
  }
}
