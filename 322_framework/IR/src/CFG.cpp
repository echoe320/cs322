#include <map> 
#include <set> 

#include "IR.h"
#include "CFG.h"

namespace IR {
  /* Node Functions */
  Node::Node(Basic_Block* b) {
    this->b = b;
  }

  /* Graph Functions */
  void Graph::addNode(Node* n){
    this->all_nodes.insert(n); 
  }
  void Graph::addEdge(Node* dst, Node* add){
    if(!this->edges.count(dst)) {  
      this->edges[dst].insert(add);
    } else {
      this->edges[dst] = {add};
    } 
  }

  Graph * create_CFG(Function* f) {
    /* Set up Graph */
    Graph* g = new Graph();

    bool check_first = false;
    for(auto b : f->basicblocks){
      Node* n_temp = new Node(b); 
      g->addNode(n_temp); 
      if (!check_first){
        g->firstnode = n_temp;
        f->entry_label = n_temp->b->label->toString();
        check_first = true; 
      }

      if (dynamic_cast<te_return *>(n_temp->b->te)){
        g->exit_nodes.insert(n_temp);
      }
    }
  }

  

}