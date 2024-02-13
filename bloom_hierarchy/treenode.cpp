#include "treenode.hpp"

void treenode::addchildren(int i, int ratio, std::vector<treenode*> nodes){
    int j=i;

    int size = static_cast<int>(nodes.size());
    while(j<i+ratio && j< size){
        this->children.push_back(nodes[j]);
        j++;
    }
}



