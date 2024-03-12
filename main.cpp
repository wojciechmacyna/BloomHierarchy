

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

    delete treeHierarchy;

}


/*
The experiments measure the avarage values from may iterations 
of the same process. Just to show if the deviation is high. 
Probably not used.
*/
void ExpOne(){

    std::string folderName = std::string("./expOne");
    std::string dbname = "ExOne";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    
    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "InBl" << "\t" << "InSST" << "HCr" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;
    int itemNumber = 1000000;

    for(int i=0; i<1; i++)
    {
        std::string dbPath = dbname + std::to_string(i);
        MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);
    }

    log.close();
}


/*
The experiment measures the values depending on the number of insertions.
*/

void ExpTwo(){

    std::string folderName = std::string("./expTwo");
    std::string dbname = "ExTwo";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;

    /*dbPath = dbname + "10mln";
    itemNumber = 10000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    
    dbPath = dbname + "50mln";
    itemNumber = 50000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    dbPath = dbname + "100mln";
    itemNumber = 100000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    dbPath = dbname + "500mln";
    itemNumber = 500000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);*/

    dbPath = dbname + "1mld";
    itemNumber = 1000000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    log.close();
}


/*
The experiment measures the values depending on the number of ration for different bloom Size
The bloom size must be changed manually
*/
void ExpThree(){

    int bloomSize = 500000;
    std::string folderName = std::string("./expThree"+ std::to_string(bloomSize));
    std::string dbname = "ExThree";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber = 100000000;;
    std::string dbPath;
    int treeRatio=5;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;


    treeRatio=3;
    dbPath = dbname + "3"+ "_"+ std::to_string(bloomSize);
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    treeRatio=5;
    dbPath = dbname + "5"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    treeRatio=10;
    dbPath = dbname + "10"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    /*treeRatio=15;
    dbPath = dbname + "15"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    treeRatio=20;
    dbPath = dbname + "20"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);*/

    log.close();
}


/*
The experiment measures the values depending on the bloom size. 
The bloom size must be changed manually in bloom_value.hpp
20000, 200000,  1000000, 2000000,
*/

void ExpFour(){

    std::string folderName = std::string("./expFour1000");
    std::string dbname = "ExFour";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;
 
    dbPath = dbname + "50mln";
    itemNumber = 50000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind);

    log.close();
}


/*
The experiments checks the difference between dbCreation with bloom and without it.
It requires levelDB changing.
*/

void ExpFive(){

    std::string folderName = std::string("./expFive");
    std::string dbname = "ExFive";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;
 
    dbPath = dbname + "1mln";
    itemNumber = 1000000;
    DBOperation::DbCreation(log, dbname, itemNumber);

    log.close();
}


void ExpSix(){

    std::string folderName = std::string("./expSix");
    std::string dbname = "ExSix";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t"  << std::endl;


    dbPath = dbname + "10mln";
    itemNumber = 10000000;
    DBOperation::DbCreation(log, dbPath, itemNumber);

    
    dbPath = dbname + "50mln";
    itemNumber = 50000000;
    DBOperation::DbCreation(log, dbPath, itemNumber);

    dbPath = dbname + "100mln";
    itemNumber = 100000000;
    DBOperation::DbCreation(log, dbPath, itemNumber);

     dbPath = dbname + "500mln";
    itemNumber = 500000000;
    DBOperation::DbCreation(log, dbPath, itemNumber); 

    log.close();
}



void ExpSeven(){

    std::string folderName = std::string("./expSeven");
    std::string dbname = "ExSix";
    std::ofstream log;
    std::string keytofind = "Key187719";
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "Key" << "\t"  << std::endl;


    dbPath = dbname + "10mln";
    DBOperation::RetrieveData(dbPath, keytofind);
   
    dbPath = dbname + "50mln";
    DBOperation::RetrieveData(dbPath, keytofind);

    dbPath = dbname + "100mln";
    DBOperation::RetrieveData(dbPath, keytofind);

    dbPath = dbname + "500mln";
    DBOperation::RetrieveData(dbPath, keytofind);

    log.close();
}


int main()
{
    //ExpOne();
    // In paper:
    ExpTwo();
    // In paper
    //ExpThree();
    //ExpFour();
    //ExpFive();
     //ExpSix();
     //ExpSeven();
     return 0;
}

