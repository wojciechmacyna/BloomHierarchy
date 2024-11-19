#pragma once

#include <string>
#include <vector>
#include "bloomTree.hpp"

class DBOperation{

    private:
        static std::string bloomExt;
        static std::string dataExt;
        static std::string dbDir;
        static std::string randomString(int length);
        static std::vector<std::string> listFilesWithExtension(const std::string& directory, const std::string& extension);
        static std::string transformFileName(std::string filename, std::string from, std::string to);

    public:
        static std::string outDir;
        static void DbCreation(std::ofstream& log, std::string dbname, int itemNmb, int percentageRandom) ;
        static void RetrieveData(std::string dbname, std::string key);
        static void Statistics(std::string dbname) ;      
        static int  RetrieveFromSStable(std::string file, std::string valueToFind);
        static void ScanningWithoutBloom(std::ofstream& log, std::string dbname, std::string valuetofind);
        static void ScanningInBloomFiles(std::ofstream& log,std::vector<std::string> bloomfiles, std::string valuetofind);
        static void ScanningWithBloom(std::ofstream& log, std::string dbname,std::string valuetofind);
        static void CreateHierarchy(std::ofstream& log, std::string dbname, bloomTree* treeHierarchy);
        static void CreateLeafHierarchyLevel(std::ofstream& log,std::string dbname, bloomTree* treeHierarchy);
        static void CheckInHierarchy(std::ofstream& log, bloomTree* treeHierarchy,std::string valuetofind);
        static void CreateBloomValue(std::ofstream& log, std::string dbname);

};