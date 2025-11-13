#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#define MAX_STRING_SIZE 10000

namespace FamilyBudget {

    template<typename T>
    class BinaryStorage {
    public:
        static bool ReadAllFromStream(std::ifstream& in, std::vector<T>& items) {
            if (!in.is_open()) {
                std::cerr << "Ошибка: поток для чтения не открыт.\n";
                return false;
            }

            items.clear();
            T item;
            while (in >> item) {
                if (in.fail()) {
                    if (!in.eof()) {
                        std::cerr << "Ошибка при чтении объекта из потока.\n";
                    }
                    break;
                }
                items.push_back(item);
            }
            return true;
        }

        static bool OverwriteAllToFile(std::ofstream& out, const std::vector<T>& items) {
            for (const auto& item : items) {
                out << item;
                if (!out) {
                    std::cerr << "Ошибка при записи объекта в файл.\n";
                    return false;
                }
            }
            out.close();
            return true;
        }

        static bool OpenDataFileForRead(std::ifstream& inputFile, const std::string& fileName) {
            inputFile.open(fileName, std::ios::binary);
            if (!inputFile.is_open()) {
                std::ofstream create(fileName, std::ios::binary | std::ios::app);
                create.close();
                inputFile.open(fileName, std::ios::binary);
                if (!inputFile.is_open()) {
                    std::cout << "Ошибка открытия файла: " << fileName << "\n";
                    return false;
                }
            }
            return true;
        }

        static bool OpenDataFileForWrite(std::ofstream& outputFile, std::ios::openmode mode, const std::string& fileName) {
            outputFile.open(fileName, std::ios::binary | mode);
            if (!outputFile.is_open()) {
                std::ofstream create(fileName, std::ios::binary | std::ios::app);
                create.close();
                outputFile.open(fileName, std::ios::binary | mode);
                if (!outputFile.is_open()) {
                    std::cout << "Ошибка открытия файла: " << fileName << "\n";
                    return false;
                }
            }
            return true;
        }


        // херня
        static bool RemoveRecord(const std::string& filename, const T& targetRecord) {
            return false;
        }

        static bool EditRecord(const std::string& filename, const T& oldRecord, const T& newRecord) {
            return false;
        }
    };
}