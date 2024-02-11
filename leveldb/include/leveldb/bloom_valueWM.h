#pragma once

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class bloom_valueWM {
 private:

  std::bitset<50000> bitArray;  // Example size
  int numHashFunctions=3;

 public:
  bloom_valueWM() {}
  void insert(const std::string& key);
  void saveToFile(const std::string& filename);

  void createFile(const std::string& filename);

  bloom_valueWM loadFromFile(const std::string& filename);

  bool exists(const std::string& key) const;
};
