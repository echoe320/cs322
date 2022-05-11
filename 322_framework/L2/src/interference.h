#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include <L2.h>

namespace L2 {
  class Node : public Item {
    public:
      Node(std::string name);
      std::string name;
      int degree = 0;
      std::string color = "none";
      std::string toString() override;
      bool isRegister = false;
      bool isSpill = false;
      bool didPop = false;
  };

  class Graph {
    public:
      Graph();
      // std::string popNode(Node*);
      std::pair<Node*, std::set<Node*>> popNode(Node*);
      bool nodeExists(std::string node_name);
      bool edgeExists(std::string src, std::string dst);
      Node* lookupNode(std::string node_name);
      void addEdge(Node* src, Node* dst);
      void printDegrees(void);
      std::unordered_map<Node*, std::set<Node*>> g;
      std::unordered_map<std::string, Node*> name_dict;
      std::map<int, std::unordered_set<Node*>> degree_dict;
      std::vector<Node*> nodes;
  };

  Graph* create_interference_graph(Function* f);
}