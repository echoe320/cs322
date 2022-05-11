#include <codegenerator.h>
#include <L3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <globalize.h>
#include <instructionSelect.h>
#include <architecture.h>

using namespace std;

extern bool is_debug; 
namespace L3{

    // string normal_var(string varname) {
    //     return "%var_" + varname.substr(1);
    // }

    string temp_var(string varname) {
        return "%tmp_" + varname.substr(1);
    }

    CodeGen::CodeGen(/*Function *f*/) {}

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
        // string line = '\t' + dst + " <- " + oprand1 + '\n'; 
        // L2_instructions.push_back(line);
        // line = '\t' + dst + " " + op + "= " + oprand2 + '\n';
        // L2_instructions.push_back(line);
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
    //TODO might need fix
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
        * Open the output file.
        */ 
       //globalize labels
        L3::labelGlobalize(&p);
        std::ofstream outputFile;
        outputFile.open("prog.L2");
        outputFile << "(:main" << endl;  
        vector<L2::Architecture::RegisterID> args = L2::Architecture::get_argument_regs();
        for(Function* f : p.functions){
            //step 1: instruction selection
            vector<string> L2_instructions = L3::instructionSelection(p, f);
            //initialize function setup 
            outputFile << "\t(" << f->name << endl; 
            outputFile << "\t\t" << f->arguments.size() << endl;
            //output arguments
            // for(int i = 0; i < min((int)f->arguments.size(), 6); i++){
            //     outputFile << "\t\t" << f->arguments[i]->toString() << " <- " << L2::Architecture::fromRegisterToString(args[i]) << "\n";
            // }
            // int count = 1; 
            // for(int i = 6; i < f->arguments.size(); i++){
            //     outputFile << "\t\tmem rsp -" << to_string(count * 8) << " <- " << f->arguments[i] << "\n";
            //     count++;
            // }
            //step 2: output L2 instructions
            for(string s : L2_instructions){
                outputFile << "\t" << s; 
            }
            outputFile << "\t)\n"; 

        }
        outputFile << ")\n" << endl;
        return;
    }
}