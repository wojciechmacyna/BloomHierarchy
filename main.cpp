

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






void MainFlow(std::ofstream& log, std::string dbname, int itemNumber, int treeRatio, std::string valuetofind){
   


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
    std::string valuetofind = "Value187719";
    
    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoblScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "HNmb" << "\t" << "HScan"<< std::endl;

    int treeRatio=5;
    int itemNumber = 1000000;

    for(int i=0; i<1; i++)
    {
        dbname = dbname + std::to_string(i);
        MainFlow(log, dbname, itemNumber, treeRatio, valuetofind);
    }

    log.close();
}



void ExpTwo(){

    std::string folderName = std::string("./expTwo");
    std::string dbname = "ExTwp";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoblScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "HNmb" << "\t" << "HScan"<< std::endl;

    int treeRatio=5;

    dbname = dbname + "10mln";
    itemNumber = 10000000;
    MainFlow(log, dbname, itemNumber, treeRatio, valuetofind);

    
    dbname = dbname + "50mln";
    itemNumber = 50000000;
    MainFlow(log, dbname, itemNumber, treeRatio, valuetofind);

    dbname = dbname + "100mln";
    itemNumber = 100000000;
    MainFlow(log, dbname, itemNumber, treeRatio, valuetofind);

    dbname = dbname + "500mln";
    itemNumber = 500000000;
    MainFlow(log, dbname, itemNumber, treeRatio, valuetofind);

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

    //ExpOne();
    ExpTwo();

 
 //   FullIndexCreation();

 //  RetrieveData();
   //Statistics();
    //findDir();

//    Statistics();


    return 0;
}

