#pragma once
#include <string>
#include "L3.h"
#include <architecture.h>
using namespace std;
namespace L3 {
    class Tile;
    class Context{
        public:
        void inline add(Instruction *i) { instructions.push_back(i);}
        bool inline isEmpty() { return instructions.size() == 0; }
        vector<Instruction *> instructions;
        int64_t start;
        int64_t end;
    };

    vector<Context *> identifyContext(Function *f);
    class TreeNode {
        public: 
        bool isroot = true;
        TreeNode(); 
        TreeNode(Item* item); 
        Item* val = nullptr; 
        Operation* op = nullptr;
        TreeNode* oprand1 = nullptr; 
        TreeNode* oprand2 = nullptr; 
        void printNode(TreeNode* node, int space);
    };
class Tree : public Visitor{
    public:
        vector<Tile *> tiles;
        TreeNode* root; 
        Tree(Instruction* i); 
        bool isEmpty() {return root == nullptr;};
        vector<Item*> uses; 
        vector<Item*> define;
        vector<string> L2_instructions;
        Instruction* getInstruction();
        void printTree(Tree* tree);

        void visit(Instruction_ret_not *i) override;
        void visit(Instruction_ret_t *i) override;
        void visit(Instruction_assignment *i) override;
        void visit(Instruction_load *i) override;
        void visit(Instruction_math *i) override;
        void visit(Instruction_store *i) override;
        void visit(Instruction_compare *i) override;
        void visit(Instruction_br_label *i) override;
        void visit(Instruction_br_t *i) override;
        void visit(Instruction_call_noassign *i) override;
        void visit(Instruction_call_assignment *i) override;
        void visit(Instruction_label *i) override;
        private: 
            Instruction* instruction;
    };
    vector<string> instructionSelection(Program p, Function* f);
}