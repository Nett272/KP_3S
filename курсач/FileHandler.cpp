#include "FileHandler.h"
#include <windows.h>
#include <iostream>
#include <algorithm>
namespace FamilyBudget {

    std::string UserFileHandler::filename = "users.bin";
    std::string FamilyFileHandler::filename = "families.bin";

    bool UserFileHandler::Registrateon(const User& user) {
        std::ifstream inputFile;
        if (!BinaryStorage<User>::openDataFileForRead(inputFile, filename)) return false;

        User tempUser;
        while (inputFile >> tempUser) {
            if (tempUser.getUsername() == user.getUsername()) {
                inputFile.close();
                return false;
            }
        }
        inputFile.close();

        std::ofstream outputFile;
        if (!BinaryStorage<User>::openDataFileForWrite(outputFile, std::ios::app, filename)) return false;
        outputFile << user;
        outputFile.close();
        return true;
    }

    bool UserFileHandler::Authorization(User& user) {
        std::ifstream inputFile;
        if (!BinaryStorage<User>::openDataFileForRead(inputFile, filename)) return false;

        User tempUser;
        while (inputFile >> tempUser) {
            if (tempUser.getUsername() == user.getUsername() &&
                tempUser.getPasswordHash() == user.getPasswordHash()) {
                inputFile.close();
                user = tempUser;
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    void UserFileHandler::PrintInfo(const User& user) {
        std::ifstream inputFile;
        if (!BinaryStorage<User>::openDataFileForRead(inputFile, filename)) return;

        User tempUser;
        while (inputFile >> tempUser) {
            if (user == tempUser) {
                std::cout << tempUser << std::endl;
                inputFile.close();
                return;
            }
        }
        inputFile.close();
        std::cout << "Пользователь не найден.\n";
    }

    void UserFileHandler::PrintFile() {
        std::ifstream inputFile;
        if (!BinaryStorage<User>::openDataFileForRead(inputFile, filename)) return;

        User tempUser;
        while (inputFile >> tempUser) {
            std::cout << tempUser << "\n";
        }
        inputFile.close();
    }

    void UserFileHandler::ClearFile() {
        std::ofstream outputFile;
        if (!BinaryStorage<Family>::openDataFileForWrite(outputFile, std::ios::trunc, filename)) return;

        outputFile.close();
        std::cout << "Файл успешно очищен: " << filename << "\n";
    }


    bool UserFileHandler::DeleteUser(std::string& userName) {
        return false; // Пока не реализовано
    }





    bool FamilyFileHandler::Registrateon(const Family& family) {
        std::ifstream inputFile;
        if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return false;

        Family tempFamily;
        while (inputFile >> tempFamily) {
            if (tempFamily.getFamilyName() == family.getFamilyName()) {
                inputFile.close();
                return false;
            }
        }
        inputFile.close();

        std::ofstream outputFile;
        if (!BinaryStorage<Family>::openDataFileForWrite(outputFile, std::ios::app, filename)) return false;
        outputFile << family;
        outputFile.close();
        return true;
    }

    bool FamilyFileHandler::Authorization(Family& family) {
        std::ifstream inputFile;
        if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return false;

        Family tempFamily;
        while (inputFile >> tempFamily) {
            if (tempFamily.getFamilyName() == family.getFamilyName() &&
                tempFamily.getPasswordHash() == family.getPasswordHash()) {
                inputFile.close();
                family = tempFamily;
                return true;
            }
        }
        inputFile.close();
        return false;
    }

    void FamilyFileHandler::PrintInfo(const User& user) {
        std::ifstream inputFile;
        if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return;

        Family tempFamily;
        while (inputFile >> tempFamily) {
            if (user.getFamilyName() == tempFamily.getFamilyName()) {
                std::cout << tempFamily << std::endl;
                inputFile.close();
                return;
            }
        }
        inputFile.close();
        std::cout << "Семья не найдена.\n";
    }

    void FamilyFileHandler::PrintFile() {
        std::ifstream inputFile;
        if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return;

        Family tempFamily;
        while (inputFile >> tempFamily) {
            std::cout << tempFamily << std::endl;
        }
        inputFile.close();
    }

    void FamilyFileHandler::ClearFile() {
        DeleteAllFamilies();
        std::ofstream outputFile;
        if (!BinaryStorage<Family>::openDataFileForWrite(outputFile, std::ios::trunc, filename)) return;

        outputFile.close();
        std::cout << "Файл успешно очищен: " << filename << "\n";
    }

    void FamilyFileHandler::ClearFamilyFile(std::string& familyName) {
        std::ofstream outputFile;
        std::string recordsFilename = Family::getDataFilename(familyName);
        if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::trunc, recordsFilename)) return;

        std::cout << "Файл успешно очищен: " << recordsFilename << "\n";
        outputFile.close();
    }

    void FamilyFileHandler::DeleteAllFamilies() {
        std::ifstream inputFile;
        if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return;

        Family tempFamily;
        while (inputFile >> tempFamily) {
            std::string fileToDelete = Family::getDataFilename(tempFamily.getFamilyName());
            DeleteFileA(fileToDelete.c_str());
        }
        inputFile.close();
    }

    void FamilyFileHandler::PrintFamilyFile(const std::string& familyName) {
        std::ifstream inputFile;
        std::string recordsFilename = Family::getDataFilename(familyName);
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, recordsFilename)) return;

        Record record;
        bool found = false;
        while (inputFile >> record) {
            std::cout << record << "\n";
        }
        inputFile.close();
    }


    bool FamilyFileHandler::DeleteFamily(std::string& familyName) {
        return false; // Пока не реализовано
    }

}