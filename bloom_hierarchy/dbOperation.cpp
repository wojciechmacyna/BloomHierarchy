
#include "dbOperation.hpp"

#include <iostream>
#include <string>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <iostream>
#include <string>
#include <vector>

#include <random>

#include <dirent.h>

//#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/bloom_valueWM.h"

#include "dbDumper.hpp"
#include "bloom_value.hpp"
#include "bloomTree.hpp"

static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


std::string DBOperation::outDir = "../out/";
std::string DBOperation::bloomExt = ".bloom";
std::string DBOperation::dataExt = ".ldb";
std::string DBOperation::dbDir = "./db/";

std::string DBOperation::randomString(int length) {
    std::string s = "";
    for (int i = 0; i < length; i++) {
        s += letters[rand() % (sizeof(letters) - 1)];
    }
    return s;
}

void DBOperation::DbCreation(std::ofstream& log, std::string dbname, int itemNumber, int percentageRandom) {
    const int num = 1;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, dbDir + dbname, &db);
    assert(status.ok());

    std::string key;
    std::string val;

    int min=1;
    int p = itemNumber/100;
    int max = p*percentageRandom;
    //long max= (percentageRandom*itemNumber)/100;
    //int max = 40000000;

     // Create a random number generator
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister engine for generating random numbers

    // Create a distribution in the range [min, max]
    std::uniform_int_distribution<> distr(min, max);


    if (percentageRandom==0){
            for (int i = num; i < itemNumber; i++) {
            key = "Key" + std::to_string(i);
            val = "Value" + std::to_string(i);
            status = db->Put(leveldb::WriteOptions(), key, val);
        }
    }
    else{
          for (int i = num; i < itemNumber; i++) {
            key = "Key" + std::to_string(i);
            int ran = distr(gen);
            val = "Value" + std::to_string(ran);
 //            std::cout << "Random number" <<  ran << std::endl;
            status = db->Put(leveldb::WriteOptions(), key, val);
        }
    }


    delete db;
    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    std::cout << "DB Creation time (ms)" << msec_diff << std::endl;
    log << std::to_string(msec_diff) <<  "\t" ;
}


void DBOperation::RetrieveData(std::string dbname, std::string key) {

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
   // options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    leveldb::Status status = leveldb::DB::Open(options, dbDir + dbname, &db);
    assert(status.ok());
    std::string document;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    db->Get(leveldb::ReadOptions(), key, &document);

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << document << "Time duration (micro seconds)" << (millisec_after - millisec_before)/1000  << std::endl; 

    delete db;
}



void DBOperation::Statistics(std::string dbname) {

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    leveldb::Status status = leveldb::DB::Open(options, dbDir + dbname, &db);
    assert(status.ok());
 

    leveldb::Slice s = "leveldb.sstables";
    std::string propertyValue;

    db->GetProperty(s, &propertyValue);

    std::cout <<  propertyValue;

    delete db;
}

// Function to list all files in a directory with a given extension
std::vector<std::string> DBOperation::listFilesWithExtension(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string filename = ent->d_name;
            if (filename.length() >= extension.length() && filename.compare(filename.length() - extension.length(), extension.length(), extension) == 0) {
                files.push_back(directory + "/" + filename);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Error opening directory" << std::endl;
    }

    return files;
}


std::string DBOperation::transformFileName(std::string filename, std::string from, std::string to){

    std::string  s = filename.substr(0, filename.length() - from.length());
    return s + to;
}


int DBOperation::RetrieveFromSStable(std::string file, std::string valueToFind){
 
    int foundValues=0;
    const std::vector<DBRecord> ssTableRecords = DBDumper::dumpSSTable(file);
    for(DBRecord r : ssTableRecords)
    {           
        std::string newValue = r.valData;
        if (newValue==valueToFind){
            std::cout << "Exists in SSTable : " + file << std::endl;
            foundValues++;
        }
    }    
    return foundValues;
}

void DBOperation::ScanningWithoutBloom(std::ofstream& log, std::string dbname, std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    int foundInSSTable=0;

    std::string directory = outDir + dbDir + dbname;

    std::vector<std::string> files = listFilesWithExtension(directory, dataExt);
    for (const auto& file : files) {

        std::string filePath = file;
        int nmbOfOccurences = RetrieveFromSStable(file, valuetofind);              
        foundInSSTable = foundInSSTable+nmbOfOccurences;
    }

    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<  "\t" ;
    std::cout << "Scanning without bloom (ms)" << msec_diff << std::endl;

}


void DBOperation::ScanningInBloomFiles(std::ofstream& log, std::vector<std::string> bloomfiles, std::string valuetofind) {

    int foundinLeafBloom=0;
    int foundInSSTable=0;

    for (const auto& file : bloomfiles) {
        
        std::string filePath = file;
        bloom_value filter = filter.loadFromFile(filePath);
        
        if (filter.exists(valuetofind)) {
            //std::cout << "Exists in bloom file : " + file << std::endl;
            foundinLeafBloom++;
            std::string datafile = transformFileName(file, bloomExt, dataExt);
            int nmbOfOccurences = RetrieveFromSStable(datafile, valuetofind);
            foundInSSTable = foundInSSTable+nmbOfOccurences;
        }      
    }
    log << std::to_string(foundinLeafBloom) <<  "\t" ;
    log << std::to_string(foundInSSTable) <<  "\t" ;
}


void DBOperation::ScanningWithBloom(std::ofstream& log, std::string dbname, std::string valuetofind){

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbDir + dbname;
    std::vector<std::string> files = listFilesWithExtension(directory, bloomExt);
    ScanningInBloomFiles(log, files, valuetofind);

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<  "\t" ;
    std::cout << "Scanning with bloom  (ms)" << msec_diff << std::endl;

}


void DBOperation::CreateHierarchy(std::ofstream& log,std::string dbname, bloomTree *treeHierarchy) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbDir + dbname;

    treeHierarchy->createTree();
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<  "\t" ;
    std::cout << "Creating hierarchy  (ms)" << msec_diff << std::endl;

}


void DBOperation::CreateLeafHierarchyLevel(std::ofstream& log, std::string dbname, bloomTree *treeHierarchy) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbDir + dbname;

    std::vector<std::string> files = listFilesWithExtension(directory, bloomExt);
    for (const auto& file : files) {
        
        //std::string filePath = file;
        bloom_value filter = filter.loadFromFile(file);
        treeHierarchy->createLeafLevel(filter, file);
          
    }
  
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<  "\t" ;
    std::cout << "Creating leaf level  (ms)" << msec_diff << std::endl;

}


void DBOperation::CheckInHierarchy(std::ofstream& log, bloomTree* treeHierarchy, std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<std::string> bloomFiles = treeHierarchy->checkExistance(valuetofind);

    if (bloomFiles.empty()){
        std::cout << "Value not found in bloom" << std::endl;
    }
    else{
        ScanningInBloomFiles(log, bloomFiles, valuetofind);

    }

    // Getting the number of hierarchy filters
    int c = treeHierarchy->GetScannedHierarchyFilters();
    log << std::to_string(c) <<  "\t";
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<   "\t" ;
    std::cout << "Checking in hierarchy  (ms)" << msec_diff << std::endl;

    // Getting bloom Accessed
    log << std::to_string(bloomFiles.size()) <<  std::endl ;

}


void DBOperation::CreateBloomValue(std::ofstream& log, std::string dbname){

    bloom_value *filter;
    std::string bloom_file;

    std::string directory = outDir + dbDir + dbname;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::vector<std::string> files = listFilesWithExtension(directory, dataExt);

    for (const std::string & file : files) {

        bloom_file = transformFileName(file, dataExt, bloomExt);
        filter = new bloom_value();
        filter->createFile(bloom_file); 
        const std::vector<DBRecord> ssTableRecords = DBDumper::dumpSSTable(file);

        for(DBRecord r : ssTableRecords)
        {           
             std::string newValue = r.valData;
             filter->insert(newValue);
        }    
        filter->saveToFile(bloom_file);

    }

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto msec_diff = (millisec_after - millisec_before) / 1000000;
    log << std::to_string(msec_diff) <<  "\t" ;
    std::cout << "Creating bloom externally  (ms)" << msec_diff << std::endl;

}