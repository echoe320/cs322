#pragma once

#include <map> 
#include <set> 
#include <stack>

#include <L2.h>
#include <liveness.h>
#include <interference.h>

namespace L2 {
  class ColorGraph {
    public:
      ColorGraph(Graph* interference_graph);
      bool coloredAny = false;
      std::string colorSelector(std::set<Node*> edges);
      void emptyGraph();
      void assignColors();
      std::stack<std::string> poppedNodes;
      std::vector<Node*> failedToColor;
      Graph* g;
  };

  ColorGraph* registerAllocate(Graph* interference_graph);
}