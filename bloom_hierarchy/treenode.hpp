#pragma once

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bloom_value.hpp"

class treenode {
 public:

    //treenode *parent;

    std::vector<treenode*> children;
    bloom_value bv;

    std::string filename;

    treenode(bloom_value bv, std::string filename){
        this->bv= bv;
        this->filename=filename;
    }

    treenode()
    {
       filename="Memory";

    }

    void addchildren(int pos, int ratio, std::vector<treenode*> nodes);

};