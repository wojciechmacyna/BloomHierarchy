

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


std::string dbname = "BigDB100mln";
std::string dbnameDest = "BigDB100mlnvvvv";


static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string randomString(int length) {
    std::string s = "";
    for (int i = 0; i < length; i++) {
        s += letters[rand() % (sizeof(letters) - 1)];
    }
    return s;
}


void FullIndexCreation() {

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, dbname, &db);
    assert(status.ok());

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    leveldb::DB* dbDest;
    leveldb::Status statusDest = leveldb::DB::Open(options, dbnameDest, &dbDest);
    assert(statusDest.ok());

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        statusDest = dbDest->Put(leveldb::WriteOptions(), it->value().ToString(), it->key().ToString());
    }

    delete db;
    delete dbDest;

    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Full index creation time (ms)" << (millisec_after - millisec_before) / 1000000;
}

void DbCreation() {
    const int num = 1;
    const int num1 = 50000000;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
   // options.filter_policy = leveldb::NewBloomFilterPolicy(10);

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
    std::cout << "DB Creation time (ms)" << (millisec_after - millisec_before) / 1000000;
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

    std::cout << document << "Time duration (ms)" << millisec_after - millisec_before;

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
    s = s + to;
    return s;
}


void RetrieveFromSStable(std::string file, std::string find_value){
 
    const std::vector<DBRecord> ssTableRecords = DBDumper::dumpSSTable(file);

    for(DBRecord r : ssTableRecords)
    {           
             std::string newValue = r.valData;
             if (newValue==find_value){
                std::cout << "Exists in SSTable : " + file << std::endl;
             }
    }    

}

void ScanningWithoutBloom(std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = "../out/" + dbname;
    std::string extension = ".ldb";

    std::vector<std::string> files = listFilesWithExtension(directory, extension);
    for (const auto& file : files) {

        std::string filePath = file;
        RetrieveFromSStable(file, valuetofind);              

    }

    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Scanning without bloom (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}



void ScanningWithBloom(std::string valuetofind) {

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string directory = "../out/" + dbname;
    std::string extension = ".bloom";

    std::vector<std::string> files = listFilesWithExtension(directory, extension);
    for (const auto& file : files) {
        
        std::string filePath = file;
        bloom_value filter = filter.loadFromFile(filePath);
        
        if (filter.exists(valuetofind)) {
            std::cout << "Exists in bloom file : " + file << std::endl;
            std::string datafile = transformFileName(file, ".bloom", ".ldb");
            RetrieveFromSStable(datafile, valuetofind);
        }      
    }

    
    auto millisec_after = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Scanning with bloom  (ms)" << (millisec_after - millisec_before)/1000000 << std::endl;

}

void CreateBloomValue(){

    bloom_value *filter;
    std::string bloom_file;

    std::string directory = "../out/" + dbname;
    std::string extension = ".ldb";
    std::vector<std::string> files = listFilesWithExtension(directory, extension);

    std::cout << "Files with extension " << extension << " in directory " << directory << ":" << std::endl;
    for (const std::string & file : files) {
        //std::cout << file << std::endl;
        bloom_file = transformFileName(file, ".ldb", ".bloom");
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

    //const std::vector<DBRecord> ssTableRecords = DBDumper::dumpSSTable(ssTable);
     //std::vector<std::vector<std::string>> ssTables = DBDumper::getSSTableFiles(primaryIndex->getLevelDbPtr(), primaryIndex->getIndexFolder());
     

 //   FullIndexCreation();

 //  RetrieveData();
   //Statistics();
    //findDir();

//    Statistics();


    return 0;
}

