#pragma once

#include <string>
#include <cstddef>

struct TestParams {
    std::string dbName;
    int numRecords;
    int bloomTreeRatio;
    int numberOfAttempts;
    size_t bloomSize;
    int numHashFunctions;
};

struct Result {
    int dbCreationTime;
    int itemNumber;
    int hierarchyFilterNumber;

    int foundInLeafBloomNoHierarchy;
    int foundInSSTableNoHierarchy;
    int foundInLeafBloomWithHierarchy;
    int foundInSSTableWithHierarchy;
    int foundInLeafBloom;
    int foundInSSTable;

    int tScanningWithoutBloom;
    int tScanningWithBloom;
    int tCreatingLeafLevel;
    int tCreatingHierarchy;
    int tCreatingBloomExternally;
    int tCheckingInHierarchy;

    int accessedBloomFilter;
    int percentage;

};