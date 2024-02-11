
#include <filesystem>
#include <iostream>
#include <string>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <thread>

#include <iostream>
#include <string>
#include <vector>




#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/bloom_valueWM.h"




std::string dbname = "BigDB100mln";
std::string dbnameDest = "BigDB100mlnDest";

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
    const int num1 = 1000000;

    auto millisec_before = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

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


int main()
{
     
   DbCreation();
 //   FullIndexCreation();

 //  RetrieveData();
   //Statistics();
    //findDir();

    Statistics();


    return 0;
}

