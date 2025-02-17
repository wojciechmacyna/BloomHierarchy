#pragma once

#include <string>
#include <vector>
#include "bloomTree.hpp"

class DBOperation{

    private:
        static std::string bloomExt;
        static std::string dataExt;
        static std::string dbDir;
        std::string randomString(int length);
        std::vector<std::string> listFilesWithExtension(const std::string& directory, const std::string& extension);
        std::string transformFileName(std::string filename, std::string from, std::string to);
        std::vector<std::string> CreateMultiplyValue(int itemNmb, int data_size);
        void CheckInHierarchyMultiplyValueForOneThread(std::ofstream& log, bloomTree* treeHierarchy, int threadNmb, std::vector<std::string> valuesToFind);

    public:
        static std::string outDir;

        void DbCreation(std::ofstream& log, std::string dbname, int itemNmb, int percentageRandom) ;
        void RetrieveData(std::string dbname, std::string key);
        void Statistics(std::string dbname) ;      
        int  RetrieveFromSStable(std::string file, std::string valueToFind);
        void ScanningWithoutBloom(std::ofstream& log, std::string dbname, std::string valuetofind);
        void ScanningInBloomFiles(std::ofstream& log,std::vector<std::string> bloomfiles, std::string valuetofind);
        void ScanningInBloomFilesMultiplyValue(std::vector<std::string> bloomfiles, std::string valuetofind);
        void ConTest(std::vector<std::string>& data, int start, int end, bloomTree* treeHierarchy);
        void ScanningWithBloom(std::ofstream& log, std::string dbname,std::string valuetofind);
        void CreateHierarchy(std::ofstream& log, std::string dbname, bloomTree* treeHierarchy);
        void CreateLeafHierarchyLevel(std::ofstream& log,std::string dbname, bloomTree* treeHierarchy);
        void CheckInHierarchy(std::ofstream& log, bloomTree* treeHierarchy,std::string valuetofind);
        void CheckInHierarchyMultiplyValue(std::ofstream& log, bloomTree* treeHierarchy, int itemNmb, int threadNmb);
        void CreateBloomValue(std::ofstream& log, std::string dbname);
        void CheckInHierarchyMultiplyValueForThreads(std::ofstream& log, bloomTree* treeHierarchy, int itemNmb, int data_size); 

};