#pragma once

#include <cmath>
#include "node.hpp"


class bloomTree {
 
 private: 
    int ratio;
    node *root;
    std::vector<node*> leafnodes;
    void createLevel(std::vector<node*> nodes);
    void traverse(node* node);
    void checkExistance(node* node, std::string value);
    void checkExistanceThread(node* n, std::string value, std::vector<std::string>& blNames);
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