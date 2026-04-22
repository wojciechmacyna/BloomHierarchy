#pragma once

#include <cmath>
#include "node.hpp"


class bloomTree {
 
 private: 
    int ratio;
    Node *root;
    std::vector<Node*> leafNodes;
    void createLevel(std::vector<Node*> Nodes);
    void traverse(Node* Node);
    void checkExistance(Node* Node, std::string value);
    void checkExistanceThread(Node* n, std::string value, std::vector<std::string>& blNames);
    int foundInHierarchy=0;
   
 public:
    bloomTree(int ratio);
    std::vector<std::string> bloomNames;
    void createLeafLevel(bloom_value bv, std::string filename);
    void createTree();
    std::vector<std::string> checkExistanceThread(std::string value);
    std::vector<std::string> checkExistance(std::string value);
    int GetScannedHierarchyFilters();

};