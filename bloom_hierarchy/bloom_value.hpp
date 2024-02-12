#pragma once

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class bloom_value {
 private:

  std::bitset<2000000> bitArray;  // Example size
  int numHashFunctions=3;

 public:
  bloom_value() {}
  void insert(const std::string& key);
  void saveToFile(const std::string& filename);

  void createFile(const std::string& filename);

  bloom_value loadFromFile(const std::string& filename);

  bool exists(const std::string& key) const;
};
