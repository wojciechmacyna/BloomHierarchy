#include "bloomTree.hpp"

bloomTree::bloomTree(int ratio){
    this->ratio = ratio;
}

void bloomTree::createLeafLevel(bloom_value bv, std::string filename){

    Node *tn = new Node(bv, filename);
    leafNodes.push_back(tn);    
    //std::cout << leafNodes.size()<< std::endl;

}

std::vector<std::string> bloomTree::checkExistance(std::string value){
   
    checkExistance(root, value);
    std::vector<std::string> ret = bloomNames;
    return ret;
}

int bloomTree::GetScannedHierarchyFilters(){
    return foundInHierarchy;
}


std::vector<std::string> bloomTree::checkExistanceThread(std::string value){
   
    std::vector<std::string> blNames;
    checkExistanceThread(root, value, blNames);
    return blNames;
}

// Refactor
void bloomTree::checkExistanceThread(Node* n, std::string value, std::vector<std::string>& blNames){
    
    // Problem here
    if (n->filename!="Memory"){
        //if (n->filename.empty()){
           blNames.push_back(n->filename);
        //   std::cout << "empty !!!!!" << std::endl;
        //}
    }
    else{
        if (n->blValue.exists(value)){
        //std::cout << "Checked in: " << n->filename << std::endl;
            for (Node* child : n->children) {
                checkExistanceThread(child, value, blNames);
            }
        }
    }
        
}


void bloomTree::checkExistance(Node* n, std::string value){
    
    // Problem here
    if (n->filename!="Memory"){
        //if (n->filename.empty()){
           bloomNames.push_back(n->filename);
        //   std::cout << "empty !!!!!" << std::endl;
        //}
    }
    else{
        if (n->blValue.exists(value)){
        //std::cout << "Checked in: " << n->filename << std::endl;
            foundInHierarchy++;
            for (Node* child : n->children) {
                checkExistance(child, value);
            }
        }
    }
        
}


void bloomTree::createTree(){
    createLevel(leafNodes);
}

void bloomTree::traverse(Node* n) {
        if (n == nullptr)
            return;

        std::cout << n->filename << std::endl;
        std::cout << "Bloom Size: " << n->blValue.bitArray.size() << std::endl;
        std::cout << "Number of 0 in Bloom: " << n->blValue.bitArray.size() - n->blValue.bitArray.count() << std::endl;

        for (Node* child : n->children) {
            traverse(child);
        }
    }

void bloomTree::createLevel(std::vector<Node*> nodes){

    std::vector<Node*> levelNodes;

    int size = static_cast<int>(nodes.size());

    double loop = ceil(size/ratio);

    for (int i=0; i<loop+1; i++){
        Node *n = new Node();
        n->addchildren(i*ratio, ratio, nodes);
        for (Node* child : n->children) {
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