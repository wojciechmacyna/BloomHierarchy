

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

#include "bloom_hierarchy/dbThreadPool.hpp"

#include "bloom_hierarchy/dbDumper.hpp"
#include "bloom_hierarchy/bloom_value.hpp"
#include "bloom_hierarchy/bloomTree.hpp"

#include "bloom_hierarchy/dbOperation.hpp"

#include "bloom_hierarchy/test_params.hpp"

#include <spdlog/spdlog.h>

DBOperation dbOper;

void writeCsvHeader(const std::string& filename,
                    const std::string& headerLine) {
  std::ofstream out(filename, std::ios::app);  // Overwrite mode
  if (!out) {
    spdlog::error(
        "Utils: Nie udało się otworzyć pliku '{}' do zapisu nagłówka!",
        filename);
    exit(1);  // Consistent with how other header functions handle errors
  }
  out << headerLine << "\n";
  out.close();
}


void MainFlowMultiplyValues(std::ofstream& log, std::string dbname, int itemNumber, int treeRatio,  int persentage, int data_size, Result& res){
   
    bloomTree *treeHierarchy = new bloomTree(treeRatio);

    dbOper.DbCreation(log, dbname, itemNumber,persentage, res);
    dbOper.CreateBloomValue(log, dbname, res);
    dbOper.CreateLeafHierarchyLevel(log, dbname,treeHierarchy, res);
    dbOper.CreateHierarchy(log, dbname,treeHierarchy, res);
    dbOper.CheckInHierarchyMultiplyValueForThreads(log, treeHierarchy, itemNumber, data_size);

    delete treeHierarchy;

}


void MainFlow(std::ofstream& log, std::string dbname, int itemNumber, int treeRatio, std::string valuetofind, int persentage, Result& res){
   
    bloomTree *treeHierarchy = new bloomTree(treeRatio);

    //log << "DBCr" << "\t" << "BlCr" << "\t" << 
    //"InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << 
    //"NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << 
    //"InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" 
    //<< "HScan"<< "\t" << "BlAccess"<< std::endl;
 
    
    //res.dbCreationTime
    dbOper.DbCreation(log, dbname, itemNumber,persentage, res); 
    //res.tCreatingBloomExternally
    dbOper.CreateBloomValue(log, dbname, res); 
    //res.tScanningWithBloom, res.foundInLeafBloomNoHierarchy, res.foundInSSTableNoHierarchy
    dbOper.ScanningWithBloom(log, dbname,valuetofind, res); 

    res.foundInLeafBloomNoHierarchy=res.foundInLeafBloom;
    res.foundInSSTableNoHierarchy=res.foundInSSTable;

    //res.tScanningWithoutBloom
    dbOper.ScanningWithoutBloom(log, dbname,valuetofind, res);  
    // res.tCreatingLeafLevel 
    dbOper.CreateLeafHierarchyLevel(log, dbname,treeHierarchy, res);
    //res.tCreatingHierarchy
    dbOper.CreateHierarchy(log, dbname,treeHierarchy, res);

    //res.foundInLeafBloomNoHierarchy, res.foundInSSTableNoHierarchy, res.tCheckingInHierarchy
    // res.hierarchyFilterNumber, res.accessedBloomFilter
    dbOper.CheckInHierarchy(log, treeHierarchy, valuetofind, res);
    res.foundInLeafBloomWithHierarchy=res.foundInLeafBloom;
    res.foundInSSTableWithHierarchy=res.foundInSSTable;

    delete treeHierarchy;

}


/*
The experiments measure the avarage values from may iterations 
of the same process. Just to show if the deviation is high. 
Probably not used.
*/
void ExpZero(){

    std::string folderName = std::string("./ExpZero");
    std::string dbname = "ExpZero";
    std::ofstream log;
    std::string valuetofind = "Value187719";

    
    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "InBl" << "\t" << "InSST" << "HCr" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;


    int itemNumber = 10000000;

    TestParams params = {dbname, itemNumber, 3, 1, 4'000'000, 3};
    Result res;

    res.itemNumber= 10000000;

    for(int i=0; i<1; i++)
    {
        std::string dbPath = dbname + std::to_string(i);
        dbOper.DbCreation(log, dbPath, itemNumber, 0, res);
    }

    std::cout << "Creation Time" <<  res.dbCreationTime << std::endl;
    log.close();

    std::ofstream out("exp_0.csv", std::ios::app);
    if (!out) {
      spdlog::error(
          "ExpBloomMetrics: Nie udało się otworzyć pliku wynikowego!");
      return;
    }
    out << res.itemNumber << "," << res.dbCreationTime  << "\n";
    out.close();
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
    Result res;

    for(int i=0; i<1; i++)
    {
        std::string dbPath = dbname + std::to_string(i);
        MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);
    }

    log.close();

    std::ofstream out("exp_0.csv", std::ios::app);
    if (!out) {
      spdlog::error(
          "ExpBloomMetrics: Nie udało się otworzyć pliku wynikowego!");
      return;
    }
    out << res.itemNumber << "," << res.dbCreationTime  << "\n";
    out.close();


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

    Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;

    dbPath = dbname + "10mln";
    itemNumber = 10000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    
    dbPath = dbname + "50mln";
    itemNumber = 50000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    dbPath = dbname + "100mln";
    itemNumber = 100000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res); 

    dbPath = dbname + "500mln";
    itemNumber = 500000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    dbPath = dbname + "1mld";
    itemNumber = 1000000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res); 

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

    Result res;

    treeRatio=3;
    dbPath = dbname + "3"+ "_"+ std::to_string(bloomSize);
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    treeRatio=5;
    dbPath = dbname + "5"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    treeRatio=10;
    dbPath = dbname + "10"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    treeRatio=15;
    dbPath = dbname + "15"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

    treeRatio=20;
    dbPath = dbname + "20"+ "_"+ std::to_string(bloomSize);;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

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
        Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;
 
    dbPath = dbname + "50mln";
    itemNumber = 50000000;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind, 0, res);

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
        Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;
 
    dbPath = dbname + "1mln";
    itemNumber = 1000000;
    dbOper.DbCreation(log, dbname, itemNumber,0, res);

    log.close();
}


void ExpSix(){

    std::string folderName = std::string("./expSix");
    std::string dbname = "ExSix";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;
        Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t"  << std::endl;

   dbPath = dbname + "10mln";
    itemNumber = 10000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 0, res);

 
   dbPath = dbname + "50mln";
    itemNumber = 50000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 0, res);

   dbPath = dbname + "100mln";
    itemNumber = 100000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 0, res);

    dbPath = dbname + "500mln";
    itemNumber = 500000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 0, res); 

    log.close();
}



void ExpSeven(){

    std::string folderName = std::string("./expSeven");
    std::string dbname = "ExTwo";
    std::ofstream log;
    std::string keytofind = "Key187719";
    std::string dbPath;


    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "Key" << "\t"  << std::endl;


    dbPath = dbname + "10mln";
    dbOper.RetrieveData(dbPath, keytofind);
   
    dbPath = dbname + "50mln";
    dbOper.RetrieveData(dbPath, keytofind);

    dbPath = dbname + "100mln";
    dbOper.RetrieveData(dbPath, keytofind);

    dbPath = dbname + "500mln";
    dbOper.RetrieveData(dbPath, keytofind);

    log.close();
}

// Scanning only value
void ExpEight(){

    std::string folderName = std::string("./expEight");
    std::string dbname = "ExTwo";
    std::ofstream log;
    std::string keytofind = "Key187719";
    std::string dbPath;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "Key" << "\t"  << std::endl;

    Result res;


    dbPath = dbname + "10mln";
    dbOper.ScanningWithoutBloom(log, dbPath, keytofind, res);
   
    dbPath = dbname + "50mln";
    dbOper.ScanningWithoutBloom(log, dbPath, keytofind, res);

    dbPath = dbname + "100mln";
    dbOper.ScanningWithoutBloom(log, dbPath, keytofind, res);

    dbPath = dbname + "500mln";
    dbOper.ScanningWithoutBloom(log, dbPath, keytofind, res);

    log.close();
}



void ExpNine(){

    std::string folderName = std::string("./expNine");
    std::string dbname = "ExNine";
    std::ofstream log;
    std::string valuetofind = "Value187719";
    int itemNumber;
    std::string dbPath;

    Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t"  << std::endl;


    dbPath = dbname + "10Proc";
    itemNumber = 10000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 10, res);

    
    dbPath = dbname + "50Proc";
    itemNumber = 10000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 50, res);

    dbPath = dbname + "80Porc";
    itemNumber = 10000000;
    dbOper.DbCreation(log, dbPath, itemNumber, 80, res);

     log.close();
}


// Next experiment

void writeExp1Headers() {
  writeCsvHeader("exp_10.csv",
                 "percentage,dbCreationTime,tCreatingBloomExternally,"
                 "tScanningWithBloom, foundInLeafBloomNoHierarchy, foundInSSTableNoHierarchy"
                 "res.tScanningWithoutBloom,res.tCreatingLeafLevel,res.tCreatingHierarchy"
                 "res.foundInLeafBloomWithHierarchy, res.foundInSSTableWithHierarchy,"
                 "res.tCheckingInHierarchy,res.hierarchyFilterNumber, res.accessedBloomFilter");


}


void writeExp10Content(const std::string& filename, Result& res){
    std::ofstream out(filename, std::ios::app);
    if (!out) {
      spdlog::error(
          "ExpBloomMetrics: Nie udało się otworzyć pliku wynikowego!");
      return;
    }
    out << res.percentage << "," << res.dbCreationTime<< "," << res.tCreatingBloomExternally  << 
    "," << res.tScanningWithBloom << "," << res.foundInLeafBloomNoHierarchy <<
    "," << res.foundInSSTableNoHierarchy << "," << 
    res.tScanningWithoutBloom << "," << res.tCreatingLeafLevel << "," << res.tCreatingHierarchy
    << "," << res.foundInLeafBloomWithHierarchy << "," << res.foundInSSTableWithHierarchy
    << "," << res.tCheckingInHierarchy << "," << res.hierarchyFilterNumber << "," 
    << res.accessedBloomFilter << "\n";
    out.close();
}

void ExpTen(){

    std::string folderName = std::string("./expTen");
    std::string dbname = "ExTen";
    std::ofstream log;
    std::string valuetofind = "Value1879";
    int itemNumber=10000000;
    int percentage;
    std::string dbPath;

    Result res;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" <<"BlScan"<< "\t" << "NoBlScan" << "\t" << "LeafCr" << "\t" << "HCr" << "\t" << "InBl" << "\t" << "InSST" << "\t" << "HNmb" << "\t" << "HScan"<< "\t" << "BlAccess"<< std::endl;

    int treeRatio=5;

    writeExp1Headers();

    dbPath = dbname + "2Proc";
    percentage=2;

    res.percentage=2;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);
    writeExp10Content("exp_10.csv", res);

    dbPath = dbname + "5Proc";
    percentage=5;
    res.percentage=5;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);

    writeExp10Content("exp_10.csv", res);

    dbPath = dbname + "10Proc";
    percentage=10;
    res.percentage=10;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);

        writeExp10Content("exp_10.csv", res);

    dbPath = dbname + "40Proc";
    percentage=40;
    res.percentage=40;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);

        writeExp10Content("exp_10.csv", res);

    dbPath = dbname + "70Proc";
    percentage=70;
    res.percentage=70;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);

        writeExp10Content("exp_10.csv", res);

    dbPath = dbname + "100Proc";
    percentage=100;
    res.percentage=100;
    MainFlow(log, dbPath, itemNumber, treeRatio, valuetofind,  percentage, res);

        writeExp10Content("exp_10.csv", res);

    log.close();
}


/*
The experiment measures multiply value search.
*/

void ExpEleven(){

    std::string folderName = std::string("./expEleven");
    std::string dbname = "ExEleven";
    std::ofstream log;
    std::string dbPath;
    int itemNumber;

    const std::string logFileName = DBOperation::outDir + folderName+ std::string("_log.txt");
    log.open(logFileName.c_str());
    log << "DBCr" << "\t" << "BlCr" << "\t"  << "LeafCr" << "\t" << "HCr" << "\t"  << "HScan"<< std::endl;

    int treeRatio=5;  
    int data_size=1000;

    itemNumber = 10000000;
    dbPath = dbname + "10mln";

    Result res;
    MainFlowMultiplyValues(log, dbPath, itemNumber, treeRatio,  0, data_size, res);

    itemNumber = 50000000;
    dbPath = dbname + "50mln";
    MainFlowMultiplyValues(log, dbPath, itemNumber, treeRatio,  0, data_size, res);


    itemNumber = 100000000;
    dbPath = dbname + "100mln";
    MainFlowMultiplyValues(log, dbPath, itemNumber, treeRatio,  0, data_size, res);

    log.close();
}



int main()
{
    //ExpZero();
    /*ExpOne();
    // In paper:
    ExpTwo();
    // In paper
    ExpThree();
    ExpFour();
    ExpFive();
    ExpSix();
    ExpSeven();
    ExpEight();
    ExpNine();
    ExpTen();
    ExpEleven();*/
    ExpTen();
     return 0;
}

