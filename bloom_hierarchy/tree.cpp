#include "tree.hpp"

void tree::createLeafLevel(bloom_value bv, std::string filename){

    treenode *tn = new treenode(bv, filename);
    leafnodes.push_back(tn);    
    //std::cout << leafnodes.size()<< std::endl;

}

std::vector<std::string> tree::checkExistance(std::string value){
    checkExistance(root, value);
    return bloomNames;
}

/*void tree::checkExistance(treenode* node, std::string value){
    
    if (node->bv.exists(value)){
        std::cout << "Checked in: " << node->filename << std::endl;
        if (node->filename!="Memory"){
            bloomNames.push_back(node->filename);
        }

        for (treenode* child : node->children) {
            checkExistance(child, value);
        }
    }
}*/


void tree::checkExistance(treenode* node, std::string value){
    
    if (node->filename!="Memory"){
            bloomNames.push_back(node->filename);
    }
    else{
        if (node->bv.exists(value)){
        std::cout << "Checked in: " << node->filename << std::endl;
            for (treenode* child : node->children) {
                checkExistance(child, value);
            }
        }
    }
        
}

void tree::calculateBitSet(treenode* node){

    if (node == nullptr)
                return;

    for (treenode* child : node->children) {
        calculateBitSet(child);
        node->bv.bitArray = node->bv.bitArray | child->bv.bitArray;

    }
}



void tree::createTree(){
    createLevel(leafnodes);
    //traverse(root);
    //calculateBitSet(root);
}

void tree::traverse(){
    traverse(root);
}

void tree::traverse(treenode* node) {
        if (node == nullptr)
            return;

        std::cout << node->filename << std::endl;
        std::cout << "Bloom Size: " << node->bv.bitArray.size() << std::endl;
        std::cout << "Number of 0 in Bloom: " << node->bv.bitArray.size() - node->bv.bitArray.count() << std::endl;

        for (treenode* child : node->children) {
            traverse(child);
        }
    }

void tree::createLevel(std::vector<treenode*> nodes){

    std::vector<treenode*> levelNodes;

    int size = static_cast<int>(nodes.size());

    double loop = ceil(size/ratio);

    for (int i=0; i<loop+1; i++){
        treenode *n = new treenode();
        n->addchildren(i*ratio, ratio, nodes);
        for (treenode* child : n->children) {
            n->bv.bitArray = n->bv.bitArray | child->bv.bitArray;
        }
        levelNodes.push_back(n);
    }

    int levelsize = static_cast<int>(levelNodes.size());
    if (levelsize>1){
        createLevel(levelNodes);
    }
    else{
        root = levelNodes[0];
    }

}