#include "dbDumper.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>

const std::string DBDumper::fileFormatStr = std::string(".ldb");



std::vector<DBRecord> DBDumper::dumpSSTable(const std::string& ssTablePath) noexcept(true)
{
    DBDumper::DBDumpFileBuffer buffer;


    leveldb::Status status = leveldb::DumpFile(leveldb::Env::Default(), ssTablePath, &buffer);
    if (!status.ok())
    {
        std::cerr << "Cannot dump ssTable: " << ssTablePath << " status: " << status.ToString() << std::endl;
        return std::vector<DBRecord>();
    }

    return buffer.getRecords();
}


leveldb::Status DBDumper::DBDumpFileBuffer::Append(const leveldb::Slice& data)
{
    //'Y+IAAA==' @ 802921 : val => 'ljkEAA==XzHHnmz2b8gc0Z3batKeTiGtl3OUZvOwSktcsO2HoWoTVOWLJOSRLGLqD02Fj8faa9UHXXAxii6XjW2sJTLnjZ6HYgLqQ5jQNtUOVY4e16t7T7L'

    const std::string line = std::string(data.ToString());

    const size_t keyFirstPos = line.find_first_of('\'') + 1;
    const size_t keyLastPos = line.substr(keyFirstPos).find_first_of('\'') - 1 + keyFirstPos;
    const std::string key = line.substr(keyFirstPos, keyLastPos - keyFirstPos + 1);

    const size_t valFirstPos = line.substr(keyLastPos + 2).find_first_of('\'') + 1 + keyLastPos + 2;
    const size_t valLastPos = line.substr(valFirstPos).find_first_of('\'') - 1 + valFirstPos;
    const std::string val = line.substr(valFirstPos, valLastPos - valFirstPos + 1);

//    LOGGER_LOG_TRACE("Getting key: ({}) val: ({}) from SSTable data ({})", key, val, data.ToString());
    fileRecords.push_back(DBRecord(key, val));

    return leveldb::Status::OK();
}

leveldb::Status DBDumper::DBDumpFileBuffer::Close()
{
    return leveldb::Status::OK();
}

leveldb::Status DBDumper::DBDumpFileBuffer::Flush()
{
    return leveldb::Status::OK();
}

leveldb::Status DBDumper::DBDumpFileBuffer::Sync()
{
    return leveldb::Status::OK();
}