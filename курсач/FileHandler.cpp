#include "FileHandler.h"
#include <windows.h>
#include <iostream>
#include <algorithm>

std::string UserFileHandler::filename = "users.bin";
std::string FamilyFileHandler::filename = "families.bin";

bool UserFileHandler::Registration(const User& user) {
    std::ifstream inputFile;
    if (!BinaryStorage<User>::openDataFileForRead(inputFile, filename)) return false;

    User tempUser;
    while (inputFile >> tempUser) {
        if (tempUser.getUsername() == user.getUsername()) {
            inputFile.close();
            std::cout << "Пользователь с таким именем уже существует.\n";
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
    std::cout << "Неверное имя пользователя или пароль.\n";
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
        std::cout << tempUser << std::endl;
    }
    inputFile.close();
}

void UserFileHandler::ClearFile() {
    std::ofstream outputFile(filename, std::ios::binary | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cout << "Ошибка очистки файла: " << filename << "\n";
    }
    else {
        std::cout << "Файл успешно очищен: " << filename << "\n";
    }
    outputFile.close();
}

bool UserFileHandler::DeleteUser(std::string& userName) {
    return false; // Пока не реализовано
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

bool FamilyFileHandler::CreateFamily(const Family& family) {
    std::ifstream inputFile;
    if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return false;

    Family tempFamily;
    while (inputFile >> tempFamily) {
        if (tempFamily.getFamilyName() == family.getFamilyName()) {
            inputFile.close();
            std::cout << "Семья с таким именем уже существует.\n";
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

bool FamilyFileHandler::AuthorizationInFamily(Family& family) {
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
    std::cout << "Неверное имя семьи или пароль.\n";
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

void FamilyFileHandler::PrintFamilyFile(const std::string& familyName) {
    std::ifstream inputFile;
    std::string recordsFilename = Family::getDataFilename(familyName);
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, recordsFilename)) {
        std::cout << "Не удалось открыть файл записей семьи: " << recordsFilename << "\n";
        return;
    }

    Record record;
    bool found = false;
    while (inputFile >> record) {
        std::cout << record << "\n";
        found = true;
    }
    if (!found) std::cout << "Файл записей семьи пуст.\n";
    inputFile.close();
}

void FamilyFileHandler::ClearFamilyFile(std::string& familyName) {
    std::ofstream outputFile;
    std::string recordsFilename = Family::getDataFilename(familyName);
    if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::trunc, recordsFilename)) {
        std::cout << "Ошибка очистки файла: " << recordsFilename << "\n";
        return;
    }
    std::cout << "Файл успешно очищен: " << recordsFilename << "\n";
    outputFile.close();
}

bool FamilyFileHandler::DeleteFamily(std::string& familyName) {
    std::vector<Family> families;
    bool found = false;

    std::ifstream inputFile;
    if (!BinaryStorage<Family>::openDataFileForRead(inputFile, filename)) return false;

    Family tempFamily;
    while (inputFile >> tempFamily) {
        if (tempFamily.getFamilyName() != familyName) {
            families.push_back(tempFamily);
        }
        else {
            found = true;
        }
    }
    inputFile.close();

    if (!found) {
        std::cout << "Семья с именем " << familyName << " не найдена.\n";
        return false;
    }

    std::ofstream outputFile;
    if (!BinaryStorage<Family>::openDataFileForWrite(outputFile, std::ios::trunc, filename)) return false;
    for (const auto& family : families) {
        outputFile << family;
    }
    outputFile.close();

    std::string recordsFile = Family::getDataFilename(familyName);
    if (remove(recordsFile.c_str()) == 0) {
        std::cout << "Файл записей семьи " << recordsFile << " также удален.\n";
    }
    else {
        std::cout << "Не удалось удалить файл записей семьи: " << recordsFile << "\n";
    }

    std::cout << "Семья " << familyName << " успешно удалена.\n";
    return true;
}

void FamilyFileHandler::ClearFile() {
    DeleteAllFamilies();
    std::ofstream outputFile;
    if (!BinaryStorage<Family>::openDataFileForWrite(outputFile, std::ios::trunc, filename)) {
        std::cout << "Ошибка очистки файла: " << filename << "\n";
        return;
    }
    outputFile.close();
    std::cout << "Файл успешно очищен: " << filename << "\n";
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