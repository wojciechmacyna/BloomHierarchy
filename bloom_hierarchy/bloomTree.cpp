#include "bloomTree.hpp"

bloomTree::bloomTree(int ratio){
    this->ratio = ratio;
}

void bloomTree::createLeafLevel(bloom_value bv, std::string filename){

    node *tn = new node(bv, filename);
    leafnodes.push_back(tn);    
    //std::cout << leafnodes.size()<< std::endl;

}

std::vector<std::string> bloomTree::checkExistance(std::string value){
    checkExistance(root, value);
    return bloomNames;
}

int bloomTree::GetScannedHierarchyFilters(){
    return foundInHierarchy;
}

void bloomTree::checkExistance(node* n, std::string value){
    
    if (n->filename!="Memory"){
            bloomNames.push_back(n->filename);
    }
    else{
        if (n->blValue.exists(value)){
        //std::cout << "Checked in: " << n->filename << std::endl;
            foundInHierarchy++;
            for (node* child : n->children) {
                checkExistance(child, value);
            }
        }
    }
        
}


void bloomTree::createTree(){

    createLevel(leafnodes);
    //traverse(root);
}

void bloomTree::traverse(node* n) {
        if (n == nullptr)
            return;

        std::cout << n->filename << std::endl;
        std::cout << "Bloom Size: " << n->blValue.bitArray.size() << std::endl;
        std::cout << "Number of 0 in Bloom: " << n->blValue.bitArray.size() - n->blValue.bitArray.count() << std::endl;

        for (node* child : n->children) {
            traverse(child);
        }
    }

void bloomTree::createLevel(std::vector<node*> nodes){

    std::vector<node*> levelNodes;

    int size = static_cast<int>(nodes.size());

    double loop = ceil(size/ratio);

    for (int i=0; i<loop+1; i++){
        node *n = new node();
        n->addchildren(i*ratio, ratio, nodes);
        for (node* child : n->children) {
            n->blValue.bitArray = n->blValue.bitArray | child->blValue.bitArray;
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