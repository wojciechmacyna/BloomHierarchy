

#include <iostream>
#include <string>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>

#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/bloom_valueWM.h"

#include "dbDumper.hpp"
#include "bloom_value.hpp"
#include "bloomTree.hpp"

std::string dbname = "BigDB";
std::string outDir = "../out/";
std::string bloomExt = ".bloom";
std::string dataExt = ".ldb";

bloomTree treeHierarchy;


static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string randomString(int length) {
    std::string s = "";
    for (int i = 0; i < length; i++) {
        s += letters[rand() % (sizeof(letters) - 1)];
    }
    return s;
}


void DbCreation() {
    const int num = 1;
    const int num1 = 50000000;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, dbname, &db);
    assert(status.ok());

    std::string key;
    std::string val;

    for (int i = num; i < num1; i++) {
        //key = randomString(15) + std::to_string(i);
        //val = randomString(70) + std::to_string(i);
        key = "Key" + std::to_string(i);
        val = "Value" + std::to_string(i);
        status = db->Put(leveldb::WriteOptions(), key, val);
    }

    delete db;
    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "DB Creation time (ms)" << (millisec_after - millisec_before) / 1000000 << std::endl;
}


void RetrieveData() {

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    leveldb::Status status = leveldb::DB::Open(options, dbname, &db);
    assert(status.ok());
    std::string document;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    db->Get(leveldb::ReadOptions(), "Key 57755", &document);

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << document << "Time duration (ms)" << (millisec_after - millisec_before)/1000000;

    delete db;
}



void Statistics() {

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    leveldb::Status status = leveldb::DB::Open(options, dbname, &db);
    assert(status.ok());
 

    leveldb::Slice s = "leveldb.sstables";
    std::string propertyValue;

    db->GetProperty(s, &propertyValue);

    std::cout <<  propertyValue;

    delete db;
}

// Function to list all files in a directory with a given extension
std::vector<std::string> listFilesWithExtension(const std::string& directory, const std::string& extension) {
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


std::string transformFileName(std::string filename, std::string from, std::string to){

    std::string  s = filename.substr(0, filename.length() - from.length());
    return s + to;
}


void RetrieveFromSStable(std::string file, std::string valueToFind){
 
    const std::vector<DBRecord> ssTableRecords = DBDumper::dumpSSTable(file);
    for(DBRecord r : ssTableRecords)
    {           
        std::string newValue = r.valData;
        if (newValue==valueToFind){
            std::cout << "Exists in SSTable : " + file << std::endl;
        }
    }    
}

void ScanningWithoutBloom(std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    //std::string directory = "../out/" + dbname;
    std::string directory = outDir + dbname;

    std::vector<std::string> files = listFilesWithExtension(directory, dataExt);
    for (const auto& file : files) {

        std::string filePath = file;
        RetrieveFromSStable(file, valuetofind);              

    }

    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Scanning without bloom (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}


void ScanningInBloomFiles(std::vector<std::string> bloomfiles, std::string valuetofind) {

    for (const auto& file : bloomfiles) {
        
        std::string filePath = file;
        bloom_value filter = filter.loadFromFile(filePath);
        
        if (filter.exists(valuetofind)) {
            std::cout << "Exists in bloom file : " + file << std::endl;
            std::string datafile = transformFileName(file, bloomExt, dataExt);
            RetrieveFromSStable(datafile, valuetofind);
        }      
    }
}


void ScanningWithBloom(std::string valuetofind){

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbname;
    std::vector<std::string> files = listFilesWithExtension(directory, bloomExt);
    ScanningInBloomFiles(files, valuetofind);

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Scanning with bloom  (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}


void CreateHierarchy() {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbname;

    treeHierarchy.createTree();
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Creating hierarchy  (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}


void CreateLeafHierarchyLevel() {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = outDir + dbname;

    std::vector<std::string> files = listFilesWithExtension(directory, bloomExt);
    for (const auto& file : files) {
        
        //std::string filePath = file;
        bloom_value filter = filter.loadFromFile(file);
        treeHierarchy.createLeafLevel(filter, file);
          
    }
  
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Creating leaf level  (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}


void CheckInHierarchy(std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::vector<std::string> bloomFiles = treeHierarchy.checkExistance(valuetofind);

    if (bloomFiles.empty()){
        std::cout << "Value not found in bloom" << std::endl;
    }
    else{
        ScanningInBloomFiles(bloomFiles, valuetofind);

    }

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Checking in hierarchy  (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}


void CreateBloomValue(){

    bloom_value *filter;
    std::string bloom_file;

    std::string directory = outDir + dbname;

    std::vector<std::string> files = listFilesWithExtension(directory, dataExt);

    for (const std::string & file : files) {

        bloom_file = transformFileName(file, ".ldb", bloomExt);
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
}

int main()
{

    DbCreation();
    CreateBloomValue();

    std::string valuetofind = "Value187719";
    ScanningWithBloom(valuetofind);   

    ScanningWithoutBloom(valuetofind);   

    CreateLeafHierarchyLevel();
    CreateHierarchy();
    CheckInHierarchy(valuetofind);

 
 //   FullIndexCreation();

 //  RetrieveData();
   //Statistics();
    //findDir();

//    Statistics();


    return 0;
}

