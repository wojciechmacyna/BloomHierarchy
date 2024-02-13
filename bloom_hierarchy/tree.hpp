#pragma once

#include <cmath>

#include "treenode.hpp"


class tree {
 
 private: 
    int ratio=5;
    treenode *root;
    std::vector<treenode*> leafnodes;
    void createLevel(std::vector<treenode*> nodes);
    void traverse();
    void traverse(treenode* node);
    void calculateBitSet(treenode* node);
    void checkExistance(treenode* node, std::string value);
   
 
 public:
    std::vector<std::string> bloomNames;
    void createLeafLevel(bloom_value bv, std::string filename);
    void createTree();
    std::vector<std::string> checkExistance(std::string value);

};