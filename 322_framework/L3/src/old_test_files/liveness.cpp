#include <liveness.h>
#include <set>
#include <algorithm>
#include <string>
#include <iostream>

extern bool is_debug;

using namespace std;

namespace L3 {

    void format_vector(const vector<set<Item*>> in, const vector<set<Item*>> out)
    {
        cout << "(" << endl;
        cout << "(in" << endl;
        for (auto i : in)
        {
            cout << "(";
            for (auto j : i)
            {
                cout << j->toString() << " ";
            }
            cout << ")" << endl;
            ;
        }
        cout << ")" << endl
             << endl;
        cout << "(out" << endl;
        for (auto i : out)
        {
            cout << "(";
            for (auto j : i)
            {
                cout << j->toString() << " ";
            }
            cout << ")" << endl;
            ;
        }
        cout << ")" << endl
             << endl;
        cout << ")" << endl;
    }
    vector<int> get_successor(vector<Instruction *> &instructions, int idx) {
        Instruction *i = instructions[idx];
        Instruction_br_label *a = dynamic_cast<Instruction_br_label *>(i);
        if (a != nullptr) {
            vector<int> successors;
            Item* label = a->label;
            int index = 0;
            while (index < instructions.size())
            {
                Instruction *curr = instructions[index];
                class Instruction_label *currLabel = dynamic_cast<class Instruction_label *>(curr);
                if (currLabel != nullptr && currLabel->label->toString() == label->toString())
                {
                    successors.push_back(index);
                }
                index++;
            }
            return successors;
        }
        // two successors
        Instruction_br_t *b = dynamic_cast<Instruction_br_t *>(i);
        if (b != nullptr)
        {
            vector<int> successors;
            Item *label = b->label;
            successors.push_back(idx + 1);
            int index = 0;
            while (index < instructions.size())
            {
                Instruction *curr = instructions[index];
                class Instruction_label *currLabel = dynamic_cast<class Instruction_label *>(curr);
                if (currLabel != nullptr && currLabel->label->toString() == label->toString())
                {
                    successors.push_back(index);
                }
                index++;
            }
            return successors;
        }
        // no successor
        Instruction_ret *c = dynamic_cast<Instruction_ret *>(i);
        if (c != nullptr)
        {
            return {};
        }
        Instruction_call *d = dynamic_cast<Instruction_call *>(i);
        if (d != nullptr)
        {
            if (String *str = dynamic_cast<String *>(d->callee)) {

                if (str->toString() == "tensor-error") {
                    return {};
                }
            }
        }
        // one successor
        if (idx == instructions.size() - 1)
        {
            return {};
        }
        return {idx + 1};
    }

    AnalysisResult* computeLiveness(Function* f) {
        AnalysisResult* res = new AnalysisResult();
        vector<set<Item*>> gens; 
        vector<set<Item*>> kills; 
        for (auto i : f->instructions)
        {
            auto gen = i->uses;
            auto kill = i->define;
            set<Item*> gen_var;
            set<Item*> kill_var;
        //    cout << "gen: ";
            for (int i = 0; i < gen.size(); i++)
            {
                // cout << gen[i]->toString() << " "; 
                gen_var.insert(gen[i]); 
            }
            gens.push_back(gen_var);
        //    cout << endl << "kill: ";
            for (int i = 0; i < kill.size(); i++)
            {
            //    cout << kill[i]->toString() << " ";
                kill_var.insert(kill[i]);
            }
        //    cout << endl;
            kills.push_back(kill_var);
        }

        vector<set<Item*>> in(f->instructions.size());
        vector<set<Item*>> out(f->instructions.size());

        bool changed = false;
        do
        {
            changed = false;
            for (int i = f->instructions.size() - 1; i >= 0; i--)
            {
                auto gen = gens[i];
                auto kill = kills[i];
                auto in_before = in[i];
                in[i] = {};

                set<Item*> diff;
                std::set_difference(out[i].begin(), out[i].end(), kill.begin(), kill.end(),
                                    std::inserter(diff, diff.end()));
                in[i].insert(gen.begin(), gen.end());
                in[i].insert(diff.begin(), diff.end());
                if (in[i] != in_before)
                {
                    changed = true;
                }

                auto out_before = out[i];
                out[i] = {};
                vector<int> idxs = get_successor(f->instructions, i);
                for (auto t : idxs)
                {
                    out[i].insert(in[t].begin(), in[t].end());
                }
                if (out[i] != out_before)
                {
                    changed = true;
                }
                Instruction* iptr = f->instructions[i];
                res->gens[iptr] = gen; 
                res->kills[iptr] = kill; 
                res->ins[iptr] = in[i];
                res->outs[iptr] = out[i];
            }
        } while (changed);
        // if(is_debug){
        //     cout << "print liveness: " << endl; 
        //     format_vector(in, out);
        // }
        return res; 
    }

    void GenKill::visit(Instruction_ret_not *i) {
        i->uses = {};
        i->define = {};
    }

    void GenKill::visit(Instruction_ret_t *i) {
        if(dynamic_cast<Variable*>(i->arg) != nullptr){
            i->uses.push_back(i->arg);
        }
        i->define = {};
    }

    void GenKill::visit(Instruction_assignment *i) {
        if(dynamic_cast<Variable*>(i->src)) i->uses.push_back(i->src);
        i->define.push_back(i->dst);
    }

    void GenKill::visit(Instruction_load *i) {
        i->uses.push_back(i->src);
        i->define.push_back(i->dst);
    }

    void GenKill::visit(Instruction_math *i) {
        if(dynamic_cast<Variable*>(i->oprand1) != nullptr) {
            i->uses.push_back(i->oprand1);
        }
        if(dynamic_cast<Variable*>(i->oprand2) != nullptr) {
            i->uses.push_back(i->oprand2);
        }
        i->define.push_back(i->dst);
    }

    void GenKill::visit(Instruction_store *i) {
        if(dynamic_cast<Variable*>(i->src)) i->uses.push_back(i->src);
        i->define.push_back(i->dst);

    }
    void GenKill::visit(Instruction_compare *i) {
        if(dynamic_cast<Variable*>(i->oprand1) != nullptr) {
            i->uses.push_back(i->oprand1);
        }
        if(dynamic_cast<Variable*>(i->oprand2) != nullptr) {
            i->uses.push_back(i->oprand2);
        }
        i->define.push_back(i->dst);
    }

    void GenKill::visit(Instruction_br_label *i) {
        i->uses = {};
        i->define = {};
    }

    void GenKill::visit(Instruction_br_t *i) {
        if(dynamic_cast<Variable*>(i->condition))i->uses.push_back(i->condition);
        i->define = {};
    }

    void GenKill::visit(Instruction_call_noassign *i) {
        for(Item* item : i->args) {
            Variable* v = dynamic_cast<Variable *>(item); 
            if(v != nullptr) i->uses.push_back(v);
        }
        Variable* v_callee = dynamic_cast<Variable*>(i->callee); 
        if(v_callee != nullptr) i->uses.push_back(v_callee);
    }

    void GenKill::visit(Instruction_call_assignment *i) {
        for(Item* item : i->args) {
            Variable* v = dynamic_cast<Variable *>(item); 
            if(v != nullptr) i->uses.push_back(v);
        }
        Variable* v_callee = dynamic_cast<Variable*>(i->callee); 
        if(v_callee != nullptr) i->uses.push_back(v_callee);
        i->define.push_back(i->dst);
    }

    void GenKill::visit(Instruction_label *i) {
        i->define = {};
        i->uses = {};
    }

}