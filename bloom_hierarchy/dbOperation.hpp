#pragma once

#include <string>
#include <vector>

#include "bloomTree.hpp"

class DBOperation{

    private:

        static std::string outDir;
        static std::string bloomExt;
        static std::string dataExt;

        static std::string randomString(int length);

    public:

        static void DbCreation(std::string dbname, int itemNmb) ;
        static void RetrieveData(std::string dbname);
        static void Statistics(std::string dbname) ;
        static std::vector<std::string> listFilesWithExtension(const std::string& directory, const std::string& extension);
        static std::string transformFileName(std::string filename, std::string from, std::string to);
        static void RetrieveFromSStable(std::string file, std::string valueToFind);
        static void ScanningWithoutBloom(std::string dbname, std::string valuetofind);
        static void ScanningInBloomFiles(std::vector<std::string> bloomfiles, std::string valuetofind);
        static void ScanningWithBloom(std::string dbname,std::string valuetofind);
        static void CreateHierarchy(std::string dbname, bloomTree* treeHierarchy);
        static void CreateLeafHierarchyLevel(std::string dbname, bloomTree* treeHierarchy);
        static void CheckInHierarchy(bloomTree* treeHierarchy,std::string valuetofind);
        static void CreateBloomValue(std::string dbname);

};