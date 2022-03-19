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
  Node::Node(std::string name) {
    this->name = name;
  }

  std::string Node::toString() {
    return "";
  }

  Graph::Graph(void) {
    std::unordered_map<Node*, std::set<Node*>> empty;
    this->g = empty;
  }
  
  // std::string Graph::popNode(Node* node) {
  std::pair<Node*, std::set<Node*>> Graph::popNode(Node* node) {
    Node* found = this->lookupNode(node->name);
    std::set<Node*> node_edges = this->g[found]; 
    for (auto it = node_edges.begin(); it != node_edges.end(); ++it) {
      Node* query = *it;
      Node* temp = this->lookupNode(query->name);
      this->degree_dict[temp->degree].erase(temp);
      temp->degree--;
      this->degree_dict[temp->degree].insert(temp);
    }
    // this->g.erase(found); // erases the node key from unordered map
    // this->name_dict.erase(found->name);
    found->didPop = true;
    this->degree_dict[found->degree].erase(found);
    std::pair<Node*, std::set<Node*>> popped{found, node_edges};
    // std::set<Node*> popped{node_edges};
    return popped;
    // return found->name;
  }

  bool Graph::nodeExists(std::string node_name) {
    auto got = this->name_dict.find(node_name);
    if (got == this->name_dict.end()) {
      return false;
    }
    else {
      return true;
    }
  }
  
  bool Graph::edgeExists(std::string src, std::string dst) {
    Node* src_node = this->lookupNode(src);
    Node* dst_node = this->lookupNode(dst);

    auto got = this->g[src_node].find(dst_node);
    if (got != this->g[src_node].end()) return true;
    else return false;
  }

  Node* Graph::lookupNode(std::string node_name) {
    if (nodeExists(node_name)) {
      return this->name_dict[node_name];
    }
    else {
      std::set<Node*> empty_set;
      Node* new_node = new Node(node_name);
      this->degree_dict[0].insert(new_node);
      this->name_dict[node_name] = new_node;
      this->g[new_node] = empty_set;
      return new_node;
    }
  }

  void Graph::addEdge(Node* src, Node* dst) {
    if (src->name != dst->name && !this->edgeExists(src->name, dst->name)) {
      this->g[src].insert(dst);
      this->g[dst].insert(src);
      this->degree_dict[src->degree].erase(src);
      this->degree_dict[dst->degree].erase(dst);
      src->degree++;
      dst->degree++;
      this->degree_dict[src->degree].insert(src);
      this->degree_dict[dst->degree].insert(dst);
    }
  }

  void Graph::printDegrees(void) {
    for (auto it = this->degree_dict.begin(); it != this->degree_dict.end(); ++it) {
      auto temp = *it;
      std::cout << std::to_string(temp.first) << ": ";
      for (auto item : temp.second) {
        std::cout << item->name << " ";
      }
      std::cout << std::endl;
    }
  }

    // COMPUTE INTERFERENCE GRAPH
  Graph* create_interference_graph(Function* f) {
    Graph* graph = new Graph();

    // connect all registers to each other
    for (int ii = 0; ii < 15; ii++) {
      std::string reg1 = all_gp_list[ii];
      Node* node1 = graph->lookupNode(reg1);
      node1->isRegister = true;
      node1->color = all_color_list[ii];
      for (int jj = 0; jj < 15; jj++) {
        std::string reg2 = all_gp_list[jj];
        Node* node2 = graph->lookupNode(reg2);
        node2->isRegister = true;
        node2->color = all_color_list[jj];
        graph->addEdge(node1, node2);
      }
    }
    
    // iterate over instructions
    for (int idx = 0; idx < f->instructions.size(); idx++) {
      // connect variables that belong to the same IN set
      for (auto it1 = f->IN[idx].begin(); it1 != f->IN[idx].end(); ++it1) {
        auto temp1 = *it1;
        Node* node1 = graph->lookupNode(temp1->toString());
        for (auto it2 = f->IN[idx].begin(); it2 != f->IN[idx].end(); ++it2) {
          auto temp2 = *it2;
          Node* node2 = graph->lookupNode(temp2->toString());
          graph->addEdge(node1, node2);
        }
      }

      // connect variables that belong to the same OUT set
      for (auto it1 = f->OUT[idx].begin(); it1 != f->OUT[idx].end(); ++it1) {
        auto temp1 = *it1;
        Node* node1 = graph->lookupNode(temp1->toString());
        for (auto it2 = f->OUT[idx].begin(); it2 != f->OUT[idx].end(); ++it2) {
          auto temp2 = *it2;
          Node* node2 = graph->lookupNode(temp2->toString());
          graph->addEdge(node1, node2);
        }
        // connect variables between KILL and OUT
        for (auto it3 = f->KILL[idx].begin(); it3 != f->KILL[idx].end(); ++it3) {
          auto temp3 = *it3;
          Node* node3 = graph->lookupNode(temp3->toString());
          graph->addEdge(node1, node3);
        }
      }

      // handle constrained arithmetic via extra edges
      auto temp_inst = dynamic_cast<Instruction_shift*>(f->instructions[idx]);
      if (temp_inst != nullptr) {
        auto shift_var = std::get<0>(temp_inst->get());
        if (dynamic_cast<Variable *>(shift_var) != nullptr) {
          auto shift_node = graph->lookupNode(shift_var->toString());
          for (auto it = graph->g.begin(); it != graph->g.end(); ++it) {
            auto temp = *it;
            if (temp.first->isRegister && temp.first->name != "rcx") graph->addEdge(shift_node, temp.first);
          }
        }
      }
    }
    // graph->printDegrees();
    // for (auto it = graph->g.begin(); it != graph->g.end(); ++it) {
    //   auto temp = *it;
    //   std::cout << temp.first->name << " ";
    //   for (auto item : temp.second) {
    //     std::cout << item->name << " ";
    //   }
    //   std::cout << std::endl;
    // }
    return graph;
  }
}