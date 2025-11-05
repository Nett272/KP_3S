#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#define MAX_STRING_SIZE 10000

template<typename T>
class BinaryStorage {
public:
    static bool ReadAllFromStream(std::ifstream& in, std::vector<T>& items);
    static bool OverwriteAllToFile(std::ofstream& out, const std::vector<T>& items);
    static bool openDataFileForRead(std::ifstream& inputFile, const std::string& fileName);
    static bool openDataFileForWrite(std::ofstream& outputFile, std::ios::openmode mode, const std::string& fileName);
};