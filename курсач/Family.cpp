#include "Family.h"
#include <algorithm>
#include <iostream>
namespace FamilyBudget {

    std::string Family::getDataFilename(const std::string& familyName) {
        return familyName + ".bin";
    }


    Family::Family(std::string familyName, std::string familyPassword)
        : familyName(familyName), passwordHash(HashPassword(familyPassword)) {
    }


    bool Family::operator==(const Family& other) const {
        return familyName == other.familyName && passwordHash == other.passwordHash;
    }


    bool Family::recordExpense(const Record& expenseRecord) {
        std::ofstream outputFile;
        if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::app, getDataFilename(familyName))) {
            return false;
        }
        outputFile << expenseRecord;
        outputFile.close();
        return true;
    }


    void Family::ShowFamilyExpenses() {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        int total = 0;

        while (inputFile >> record) {
            std::cout << record << "\n";
            found = true;
        }

        std::cout << "Общая сумма расходов семьи: " << total << "\n";

        inputFile.close();
    }


    bool Family::deleteRecord(const Record& targetRecord) {
        return false;
    }

    bool Family::editRecord(const Record& targetRecord, const Record& updatedRecord) {
        return false;
    }


    bool Family::sortRecords(std::function <bool(const Record&, const Record&)> comp) {
        std::vector<Record> records;
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return false;
        if (!BinaryStorage<Record>::ReadAllFromStream(inputFile, records)) return false;
        inputFile.close();

        if (records.empty()) return true;

        std::sort(records.begin(), records.end(), comp);

        std::ofstream outputFile;
        if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::trunc, getDataFilename(familyName))) return false;
        return BinaryStorage<Record>::OverwriteAllToFile(outputFile, records);
    }

    void Family::FindRecord(std::function <bool(const Record&)> comp) const {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        while (inputFile >> record) {
            if (comp(record)) {
                std::cout << record << "\n";
                found = true;
            }
        }
        if (!found) std::cout << "Записей с такими параметрами не найдена.\n";
        inputFile.close();
    }


    void Family::FilterByAmountRange(int minAmount, int maxAmount) const {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        while (inputFile >> record) {
            int amount = record.getAmount();
            if (amount >= minAmount && amount <= maxAmount) {
                std::cout << record << '\n';
                found = true;
            }
        }
        if (!found) std::cout << "Нет записей в указанном диапазоне сумм.\n";
        inputFile.close();
    }

    void Family::FilterByUserName(const std::string& username) const {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        while (inputFile >> record) {
            auto userPtr = record.getUser();
            if (userPtr && userPtr->getUsername() == username) {
                std::cout << record << '\n';
                found = true;
            }
        }
        if (!found) std::cout << "Записей этого пользователя не найдено.\n";
        inputFile.close();
    }

    void Family::FilterByDateRange(const std::string& startDate, const std::string& endDate) const {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        while (inputFile >> record) {
            const std::string& recordDate = record.getDate();
            if (recordDate >= startDate && recordDate <= endDate) {
                std::cout << record << '\n';
                found = true;
            }
        }
        if (!found) std::cout << "Нет записей в указанном диапазоне дат.\n";
        inputFile.close();
    }

    // херня
    void Family::generateReport(const std::string& startDate, const std::string& endDate) const {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        std::map<std::string, std::map<std::string, int>> userCategorySums;
        std::map<std::string, int> familyCategorySums;
        int totalFamilySum = 0;

        Record record;
        while (inputFile >> record) {
            const std::string& date = record.getDate();
            if (date < startDate || date > endDate) continue;

            std::shared_ptr<User> userPtr = record.getUser();
            std::string username = userPtr ? userPtr->getUsername() : "Неизвестный";
            int amount = record.getAmount();
            std::string category = record.getCategory();

            userCategorySums[username][category] += amount;
            familyCategorySums[category] += amount;
            totalFamilySum += amount;
        }
        inputFile.close();

        std::cout << "\n=== ОТЧЁТ О ТРАТАХ СЕМЬИ \"" << familyName << "\" ===\n";
        std::cout << "Период: " << startDate << " — " << endDate << "\n\n";

        if (userCategorySums.empty()) {
            std::cout << "За указанный период нет данных.\n";
            return;
        }

        for (const auto& userPair : userCategorySums) {
            std::cout << "Пользователь: " << userPair.first << "\n";
            for (const auto& categoryPair : userPair.second) {
                std::cout << "  Категория: " << categoryPair.first
                    << " | Сумма: " << categoryPair.second << "\n";
            }
            std::cout << std::endl;
        }

        std::cout << "=== Общие траты семьи по категориям ===\n";
        for (const auto& categoryPair : familyCategorySums) {
            std::cout << "  Категория: " << categoryPair.first
                << " | Всего: " << categoryPair.second << "\n";
        }

        std::cout << "\n=== Общая сумма трат семьи ===\n";
        std::cout << "Итого: " << totalFamilySum << "\n\n";
    }


    void Family::ShowMyExpenses(const User& currentUser) {
        std::ifstream inputFile;
        if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

        bool found = false;
        Record record;
        int total = 0;

        while (inputFile >> record) {
            if (record.getUser() && record.getUser()->getUsername() == currentUser.getUsername()) {
                std::cout << record << "\n";
                total += record.getAmount();
                found = true;
            }
        }

        std::cout << "Общая сумма ваших расходов: " << total << "\n";

        inputFile.close();
    }

    std::ofstream& operator<<(std::ofstream& out, const Family& family) {
        size_t nameSize = family.familyName.size();
        out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        out.write(family.familyName.c_str(), nameSize);

        size_t hashSize = family.passwordHash.size();
        out.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
        out.write(family.passwordHash.c_str(), hashSize);

        return out;
    }

    std::ifstream& operator>>(std::ifstream& in, Family& family) {
        size_t nameSize = 0;
        if (!in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize)) || nameSize > MAX_STRING_SIZE) {
            in.setstate(std::ios::failbit);
            return in;
        }
        family.familyName.resize(nameSize);
        if (!in.read(&family.familyName[0], nameSize)) return in;

        size_t hashSize = 0;
        if (!in.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize)) || hashSize > MAX_STRING_SIZE) {
            in.setstate(std::ios::failbit);
            return in;
        }
        family.passwordHash.resize(hashSize);
        if (!in.read(&family.passwordHash[0], hashSize)) return in;

        return in;
    }


    std::ostream& operator<<(std::ostream& out, const Family& family) {
        out << "Имя семьи: " << family.familyName << "\n";
        out << "Хэш пароля семьи: " << family.passwordHash << "\n";
        // вывод всех членов семьи 
        return out;
    }
}