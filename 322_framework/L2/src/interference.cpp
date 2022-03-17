#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <interference.h>

// included libraries
#include <unordered_set>
#include <unordered_map>

std::string all_gp_list[] = {"r10", "r11", "r8", "r9", "rax", "rcx", "rdi", "rdx", "rsi", "r12", "r13", "r14", "r15", "rbp", "rbx"};

namespace L2 {
    Graph::Graph(void) {
        std::unordered_map<std::string, std::set<std::string>> empty;
        this->g = empty;
    }

    std::unordered_map<std::string, std::set<std::string>> Graph::getMap() {
        return this->g;
    }

    bool Graph::nodeExists(std::string node) {
        auto got = this->g.find(node);
        if (got == this->g.end()) {
            return false;
        }
        else {
            return true;
        }
    }

    void Graph::addEdge(std::string src, std::string dst) {
        if (src != dst) {
            std::set<std::string> empty_set;
            if (!nodeExists(src)) {
                this->g[src] = empty_set;
            }
            if (!nodeExists(dst)) {
                this->g[dst] = empty_set;
            }
            this->g[src].insert(dst);
            this->g[dst].insert(src);
        }
    }

    void Graph::printGraph(void) {
        for (auto it = this->g.begin(); it != this->g.end(); ++it) {
            std::cout << it->first << " ";
            for (auto item : it->second) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    }

    // COMPUTE INTERFERENCE GRAPH
    void create_interference_graph(Program p) {
        for (auto f : p.functions) {
            Graph* graph = new Graph();

            // connect all registers to each other
            int num_reg = sizeof(all_gp_list)/sizeof(all_gp_list[0]);
            for (auto reg1 : all_gp_list) {
                for (auto reg2 : all_gp_list) {
                    graph->addEdge(reg1, reg2);
                }
            }

            // iterate over instructions
            for (int idx = 0; idx < f->instructions.size(); idx++) {
                // connect variables that belong to the same IN set
                for (auto it1 = f->IN[idx].begin(); it1 != f->IN[idx].end(); ++it1) {
                    auto temp1 = *it1;
                    for (auto it2 = f->IN[idx].begin(); it2 != f->IN[idx].end(); ++it2) {
                        auto temp2 = *it2;
                        graph->addEdge(temp1->toString(), temp2->toString());
                    }
                }

                // connect variables that belong to the same OUT set
                for (auto it1 = f->OUT[idx].begin(); it1 != f->OUT[idx].end(); ++it1) {
                    auto temp1 = *it1;
                    for (auto it2 = f->OUT[idx].begin(); it2 != f->OUT[idx].end(); ++it2) {
                        auto temp2 = *it2;
                        graph->addEdge(temp1->toString(), temp2->toString());
                    }
                    // connect variables between KILL and OUT
                    for (auto it3 = f->KILL[idx].begin(); it3 != f->KILL[idx].end(); ++it3) {
                            auto temp3 = *it3;
                            graph->addEdge(temp1->toString(), temp3->toString());
                    }
                }

                // handle constrained arithmetic via extra edges
                auto temp_inst = dynamic_cast<Instruction_shift*>(f->instructions[idx]);
                if (temp_inst != nullptr) {
                    auto shift_var = std::get<0>(temp_inst->get());
                    if (dynamic_cast<Variable *>(shift_var) != nullptr) {
                        for (auto reg : all_gp_list) {
                            if (reg != "rcx") graph->addEdge(shift_var->toString(), reg);
                        }
                    }
                }
            }
            graph->printGraph();
        }
    }
}