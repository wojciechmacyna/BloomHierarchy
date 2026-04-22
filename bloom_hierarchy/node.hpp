#pragma once

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bloom_value.hpp"

class Node {
 public:

    std::vector<Node*> children;
    bloom_value blValue;
    std::string filename;

    Node(bloom_value bv, std::string filename){
        this->blValue= bv;
        this->filename=filename;
    }

    Node()
    {
       filename="Memory";
    }

    void addchildren(int pos, int ratio, std::vector<Node*> nodes);
};