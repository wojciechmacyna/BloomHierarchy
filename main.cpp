

#include <iostream>
#include <string>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <dirent.h>

#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/bloom_valueWM.h"

#include "bloom_hierarchy/dbDumper.hpp"
#include "bloom_hierarchy/bloom_value.hpp"
#include "bloom_hierarchy/bloomTree.hpp"

#include "bloom_hierarchy/dbOperation.hpp"






void MainFlow(std::ofstream& log, std::string dbname){
    std::string valuetofind = "Value187719";

    int treeRatio=5;
    int itemNumber = 50000000;
    bloomTree *treeHierarchy = new bloomTree(treeRatio);

    //std::string directory = outDir + dbDir + dbname;
  
    DBOperation::DbCreation(log, dbname, itemNumber);
    DBOperation::CreateBloomValue(log, dbname);
    DBOperation::ScanningWithBloom(log, dbname,valuetofind);   
    DBOperation::ScanningWithoutBloom(log, dbname,valuetofind);   
    DBOperation::CreateLeafHierarchyLevel(log, dbname,treeHierarchy);
    DBOperation::CreateHierarchy(log, dbname,treeHierarchy);
    DBOperation::CheckInHierarchy(log, treeHierarchy, valuetofind);

}


void ExpOne(){

    std::string folderName = std::string("./expOne");
    std::string dbname = "ExOne";
    std::ofstream log;
    
    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DB_Cr Bloom_Cr Bloom_scan No_bloom_scan Leaf_Cr H_Cr H_Scan "<< std::endl;

    for(int i=0; i<10; i++)
    {
        dbname = dbname + std::to_string(i);
        MainFlow(log, dbname);
    }

    log.close();

}


int main()
{

    /*DbCreation();
    CreateBloomValue();

    std::string valuetofind = "Value187719";
    ScanningWithBloom(valuetofind);   

    ScanningWithoutBloom(valuetofind);   

    CreateLeafHierarchyLevel();
    CreateHierarchy();
    CheckInHierarchy(valuetofind);*/

    ExpOne();

 
 //   FullIndexCreation();

 //  RetrieveData();
   //Statistics();
    //findDir();

//    Statistics();


    return 0;
}

