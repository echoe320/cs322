#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace L1{
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

    const char *callee_reg[]

    outputFile << "

    outputFile << "pushq

    //for (auto func : p.functions) {
    //  outputFile << "\t_" << func->name << "\n";
    //  for (auto i : func->instructions) {
    //    outputFile << "" << "\n";
    //  }
    //}
    
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
