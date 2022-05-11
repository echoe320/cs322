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

    if (labels.size() == 0)
        return;
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

  CodeGen::CodeGen(/*Function *f*/) {}

  /* Defining Visitor Functions */

  void CodeGen::visit(Tile_return* t){
      TreeNode *tree = t->getTree();
      L2_instructions.push_back("\treturn\n");
  }
  void CodeGen::visit(Tile_return_t* t){
      TreeNode *tree = t->getTree();
      Variable* arg = dynamic_cast<Variable*>(tree->val); 
      string line;
      if(arg){
          line = "\t" + t->root->oprand1->matched_node->val->toString() + " <- rdi\n";
          L2_instructions.push_back(line);
      }
      line = "\trax <- " + t->root->oprand1->matched_node->val->toString() + "\n";
      L2_instructions.push_back(line);
      L2_instructions.push_back("\treturn\n");
  }
  void CodeGen::visit(Tile_assign* t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString();
      string oprand = t->root->oprand1->matched_node->val->toString();
      string line = "\t" + dst + " <- " + oprand + '\n';
      L2_instructions.push_back(line);
  }

  void CodeGen::visit(Tile_math* t) {
      TreeNode *tree = t->getTree();
      if (!tree) cout << "bug " << endl;
      string dst = tree->val->toString();
      string oprand1 = t->root->oprand1->matched_node->val->toString();
      string op = tree->op->toString(); 
      string oprand2 = t->root->oprand2->matched_node->val->toString();
      if (op == "*" || op == "+" || op == "&") {
          string line = '\t' + dst + " <- " + oprand1 + '\n'; 
          L2_instructions.push_back(line);
          line = '\t' + dst + " " + op + "= " + oprand2 + '\n';
          L2_instructions.push_back(line);
      } else if (op == "-" || op == "<<" || op == ">>") {
          string tmp = temp_var("%tmp");
          string line = '\t' + tmp + " <- " + oprand1 + '\n'; 
          L2_instructions.push_back(line);
          line = '\t' + tmp + " " + op + "= " + oprand2 + '\n'; 
          L2_instructions.push_back(line);
          line = '\t' + dst + " <- " + tmp + '\n'; 
          L2_instructions.push_back(line);
      }

  }

  void CodeGen::visit(Tile_math_specialized* t) {
      string dst = t->root->matched_node->val->toString();

      string other = t->root->oprand2->matched_node->val->toString();
      string op = t->root->op->get();

      string line = '\t' + dst + " " + op + "= " + other + '\n';   
      L2_instructions.push_back(line);
  }


  void CodeGen::visit(Tile_compare *t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString(); 
      string oprand1 = t->root->oprand1->matched_node->val->toString();
      string op = tree->op->toString(); 
      string oprand2 = t->root->oprand2->matched_node->val->toString();
      string line;
      
      if(op == ">="){
          line = '\t' + dst + " <- " + oprand2 + " <= " + oprand1 +  '\n'; 
      }
      else if(op == ">"){
          line = '\t' + dst + " <- " + oprand2 + " < " + oprand1 +  '\n'; 
      }
      else {
          line = '\t' + dst + " <- " + oprand1 + " " + op + " " + oprand2 +  '\n'; 
      }
      L2_instructions.push_back(line);
  }

  void CodeGen::visit(Tile_load *t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString(); 
      string oprand1 = t->root->oprand1->matched_node->val->toString();
      //need to know current number of item on stack 
      // string M = to_string(this->f->sizeOfStack * 8);
      // string line = '\t' + dst + " <- mem " + oprand1 + " " + M + '\n'; 

      string line = '\t' + dst + " <- mem " + oprand1 + " 0\n"; 
      L2_instructions.push_back(line);
  }
  void CodeGen::visit(Tile_store *t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString(); 
      string oprand1 = t->root->oprand1->matched_node->val->toString();
      //need to know current number of item on stack 
      string line = "\tmem" + dst + " 0 <- " + oprand1 + '\n'; 
      L2_instructions.push_back(line);
  }
  void CodeGen::visit(Tile_br* t){
    TreeNode *tree = t->getTree();
    string label = t->root->oprand1->matched_node->val->toString();
    string line = "\tgoto " + label + '\n'; 
    L2_instructions.push_back(line);
  }
  void CodeGen::visit(Tile_br_t* t){
      TreeNode *tree = t->getTree();
      while (!(tree->oprand1 && tree->oprand2)) {
          if (tree->oprand1) tree = tree->oprand1;
          if (tree->oprand2) tree = tree->oprand2;
      }
      string label = tree->oprand2->val->toString(); 
      Item* condition = tree->oprand1->val;
      Number* n = dynamic_cast<Number*>(condition); 
      string line; 
      if(n != nullptr){
          if(n->get() == 0) line = "\tcjump 0 = 1 " + label + '\n'; 
          else line = "\tcjump 1 = 1 " + label + '\n'; 
      }
      else {
          //condition is a variable, find the two nodes in the tree that define this variable
          line = "\tcjump " + condition->toString() + " = 1 " + label + "\n";   
      }
      L2_instructions.push_back(line);
  }

  void CodeGen::visit(Tile_increment* t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString(); 
      string line; 
      if(tree->op->toString() == "+"){
          line = "\t" + dst + "++\n";
      }
      else {
          line = "\t" + dst + "--\n";
      }
      L2_instructions.push_back(line);
  }

  void CodeGen::visit(Tile_at* t){
      TreeNode *tree = t->getTree();
      string dst = tree->val->toString(); 
      string src_add = t->root->oprand2->matched_node->val->toString(); 
      string src_mult = t->root->oprand1->oprand1->matched_node->val->toString(); 
      string src_const = t->root->oprand1->oprand2->matched_node->val->toString(); 
      string line = "\t" + dst + " @ " + src_add + " " + src_mult + " " + src_const + "\n";
      L2_instructions.push_back(line);       
  }

  void generate_code(Program p){
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

      /* Instruction select */
      vector<string> L2_instructions = L3::instructionSelection(p, f);
      
      for(string s : L2_instructions) {
        outputFile << "\t" << s; 
      }
      
      /* end of function */
      outputFile << "\t)\n"; 
    }
    /* close main function */
    outputFile << ")\n\n";
  }
}