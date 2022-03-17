#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include <L2.h>

namespace L2 {
    void create_interference_graph(Program p);

    class Graph {
        public:
            Graph();
            std::unordered_map<std::string, std::set<std::string>> getMap();
            bool nodeExists(std::string node);
            void addEdge(std::string src, std::string dst);
            void printGraph(void);

        private:
            std::unordered_map<std::string, std::set<std::string>> g;
    };
}