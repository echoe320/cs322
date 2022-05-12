#include "IR.h"

namespace IR {
  /* Node */
  struct Node {
    public:
      Node(Basic_Block * b);
      Basic_Block * b;
  };

  /* Graph */
  struct Graph {
    public:
      void addNode(Node* n); 
      void addEdge(Node* dst, Node* add); 

      Node* firstnode;
      std::set<Node *> exit_nodes;
      std::set<Node *> all_nodes;
      std::map<Node*, std::set<Node*>> edges;
  };

  /* Trace */
  struct Trace {

  };

}