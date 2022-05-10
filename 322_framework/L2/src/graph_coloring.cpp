#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <graph_coloring.h>
#include <L2.h>

// in this order: r10, r11, r8, r9, rax, rcx, rdi, rdx, rsi, r12, r13, r14, r15, rbp, rbx
// std::string all_color_list[] = {"red", "orange", "yellow", "green", "blue", "indigo", "purple", "pink", "brown", "black", "white", "gray", "maroon", "navy", "periwinkle"};

namespace L2 {
  ColorGraph::ColorGraph(Graph* interference_graph) {
    this->g = interference_graph;
  }

  std::string ColorGraph::colorSelector(std::set<Node*> edges) {
    // for (int idx = 0; idx < 15; idx++) {
    for (auto i : all_color_list) {
      bool color_present = false;
      for (auto it = edges.begin(); it != edges.end(); ++it) {
        //// auto temp = *it;
        //// Node* adj_node = this->g->lookupNode(temp->name);
        Node* adj_node = *it;
        if (adj_node->color == i) { 
          color_present = true;
          break;
        }
        // if (color == adj_node->color) break;
      }
      if (color_present == false) return i;
    }
    return "spill";
  }

  //* STEP 1) Repeatedly select a node and remove it from the graph, putting it on top of the stack
  //! lets just "pop" the node names so we can keep the edges and look up nodes
  // NOTES TO CATCH EUGENE UP:
  // so step 1 was working (see commented version right under this one)
  // but the problem was when i started to color the graphs, because we
  // physically removed the node from the graph, it was difficult to keep
  // track of the edges between nodes (to check for color conflicts).
  // so i tried not actually popping the nodes and just "popping" the names
  // of the nodes, but i cant get that to work either. to finish graph coloring
  // we need to assign colors to each (non-register) node so that the colors
  // dont conflict with any connected nodes. im gonna take a nap so i can get
  // some studying done for my final. if you cant figure this out by the time
  // im done with my final, i think we're doomed and we just take the L :(
  // void ColorGraph::emptyGraph() {
  //   int deg = 14;
  //   while (deg >= 0) {
  //     auto curr_nodes = this->g->degree_dict[deg];
  //     for (auto it = curr_nodes.begin(); it != curr_nodes.end(); ++it) {
  //       auto temp = *it;
  //       if (!temp->isRegister) {
  //         std::string popped = this->g->popNode(temp); //* popped is a pair<Node*, unordered_set<Node*>>
  //         this->poppedNodes.push(popped);
  //       }
  //     }
  //     deg--;
  //   }
  //   deg = this->g->degree_dict.size() - 1;
  //   while (deg >= 0) {
  //     auto curr_nodes = this->g->degree_dict[deg];
  //     for (auto it = curr_nodes.begin(); it != curr_nodes.end(); ++it) {
  //       auto temp = *it;
  //       if (!temp->isRegister) {
  //         std::string popped = this->g->popNode(temp); //* popped is a pair<Node*, unordered_set<Node*>>
  //         this->poppedNodes.push(popped);
  //       }
  //     }
  //     deg--;
  //   }
  // }
  void ColorGraph::emptyGraph() {
    int deg = 14;
    while (deg >= 0) {
      auto curr_nodes = this->g->degree_dict[deg];
      for (auto it = curr_nodes.begin(); it != curr_nodes.end(); ++it) {
        auto query = *it;
        auto temp = this->g->lookupNode(query->name);
        if (!temp->isRegister && !temp->didPop) {
          auto popped = this->g->popNode(temp);
          this->poppedNodes.push(popped.first);
          this->Node_neighbors.push(popped.second);
          // auto popped = this->g->popNode(temp); //* popped is a pair<Node*, unordered_set<Node*>>
        }
      }
      deg--;
    }
    deg = this->g->degree_dict.size() - 1;
    while (deg >= 0) {
      auto curr_nodes = this->g->degree_dict[deg];
      for (auto it = curr_nodes.begin(); it != curr_nodes.end(); ++it) {
        auto query = *it;
        auto temp = this->g->lookupNode(query->name);
        if (!temp->isRegister && !temp->didPop) {
          auto popped = this->g->popNode(temp);
          this->poppedNodes.push(popped.first);
          this->Node_neighbors.push(popped.second);
        //   auto popped = this->g->popNode(temp); //* popped is a pair<Node*, unordered_set<Node*>>
        //   this->poppedNodes.push(popped);
        }
      }
      deg--;
    }
  }

  //* STEP 2) Select a color on each node as it comes back into the graph, making sure no adjacent nodes have the same color
  void ColorGraph::assignColors() {
    while (!this->poppedNodes.empty()) {
      // Node* popped = this->g->lookupNode(this->poppedNodes.top());
      Node* popped = this->poppedNodes.top();
      // std::set<Node*> n = this->Node_neighbors.top();
      //// popped = this->g->lookupNode(popped->name);
      // std::set<Node*> popped_neighbors = this->g->g[popped];
      std::set<Node*> popped_neighbors = Node_neighbors.top();
      popped->color = this->colorSelector(popped_neighbors);
      popped->didPop = false;
      // std::cout << popped->name << " " << popped->color << ": ";
      for (auto it = popped_neighbors.begin(); it != popped_neighbors.end(); ++it) {
        auto temp = *it;
        auto adj_node = this->g->lookupNode(temp->name);
        auto got = this->g->g[adj_node].find(popped);
        auto pop_ref = *got;
        pop_ref->color = popped->color;
      }
      // std::cout << std::endl;
      // std::cout << popped->name << " is " << popped->color << std::endl;
      if (popped->color == "spill") {
        this->failedToColor.push_back(popped);
        // popped->isSpill = true;
      }
      else this->coloredAny = true;
      this->poppedNodes.pop();
      this->Node_neighbors.pop();
    }
  }

  //* ACTUAL FUNCTION THAT CALLS THE OTHER HELPERS
  ColorGraph* registerAllocate(Graph* interference_graph) {
    ColorGraph* cgraph = new ColorGraph(interference_graph);
    cgraph->emptyGraph();
    cgraph->assignColors();

    // std::cout << "variables to be spilled: ";
    for (auto node : cgraph->failedToColor) {
      // std::cout << node->name << " ";
      Variable* temp_var = new Variable(node->name);
      cgraph->tobeSpilled.push_back(temp_var);
    }
    // std::cout << std::endl;
    return cgraph;
  }

}