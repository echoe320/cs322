#include <algorithm>
#include <iostream>
#include <globalize.h>

extern bool is_debug;

namespace L3 {
    bool comp(Label *l1, Label *l2) {
        return l1->get().length() > l2->get().length();
    }

    void labelGlobalize(Program *p) {
        vector<Label *> labels;
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
        sort(labels.begin(), labels.end(), comp);


        ::string llg = labels[0]->get().substr(1);
        p->ll = llg;
        llg = llg + "_global_";

        for (auto f : p->functions) {
            ::string fname = f->name.substr(1);
            for (auto i : f->instructions) {
                Instruction_label *l = dynamic_cast<Instruction_label *>(i);
                if (l) {
                    ::string old = l->label->get().substr(1);
                    l->label = new Label(":" + llg + fname + "_" + old);
                    if(is_debug) cout << "label: " << l->label->get() << endl;
                }
                Instruction_br_label *bl = dynamic_cast<Instruction_br_label *>(i);
                if(bl) {
                    ::string old = bl->label->get().substr(1);
                    bl->label = new Label(":" + llg + fname + "_" + old);
                    if(is_debug) cout << "br label: " << bl->label->get() << endl;                    
                }
                Instruction_br_t *bt = dynamic_cast<Instruction_br_t *>(i);
                if(bt) {
                    ::string old = bt->label->get().substr(1);
                    bt->label = new Label(":" + llg + fname + "_" + old);
                    if(is_debug) cout << "bt label: " << bt->label->get() << endl;                    
                }
                Instruction_assignment *a = dynamic_cast<Instruction_assignment *>(i);
                if(a) {
                    Label* label = dynamic_cast<Label*>(a->src);
                    if(label) {
                        ::string old = label->get().substr(1);
                        label = new Label(":" + llg + fname + "_" + old);
                        if(is_debug) cout << "assign label: " << label->get() << endl; 
                    }                   
                }
            }
        }
    }
}