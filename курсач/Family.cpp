#include "Family.h"
#include <algorithm>
#include <iostream>
using namespace FamilyBudget;

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

void Family::printInfo() {
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

    Record record;
    while (inputFile >> record) {
        std::cout << record << std::endl;
    }
    inputFile.close();
}

bool Family::deleteRecord(const Record& targetRecord) {
    std::vector<Record> records;
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return false;
    if (!BinaryStorage<Record>::ReadAllFromStream(inputFile, records)) return false;
    inputFile.close();

    auto it = std::find_if(records.begin(), records.end(),
        [&](const Record& r) { return r == targetRecord; });
    if (it == records.end()) return false;

    records.erase(it);

    std::ofstream os;
    if (!BinaryStorage<Record>::openDataFileForWrite(os, std::ios::trunc, getDataFilename(familyName))) return false;
    bool result = BinaryStorage<Record>::OverwriteAllToFile(os, records);
    return result;
}

bool Family::editRecord(const Record& targetRecord, const Record& updatedRecord) {
    std::vector<Record> records;
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return false;
    if (!BinaryStorage<Record>::ReadAllFromStream(inputFile, records)) return false;
    inputFile.close();

    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i] == targetRecord) {
            records[i] = updatedRecord;
            std::ofstream outputFile;
            if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::trunc, getDataFilename(familyName))) return false;
            return BinaryStorage<Record>::OverwriteAllToFile(outputFile, records);
        }
    }
    return false;
}

bool Family::sortRecords(SortField sortBy, bool ascending) {
    std::vector<Record> records;
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return false;
    if (!BinaryStorage<Record>::ReadAllFromStream(inputFile, records)) return false;
    inputFile.close();

    if (records.empty()) return true;

    switch (sortBy) {
    case SortField::DATE:
        std::sort(records.begin(), records.end(),
            [ascending](const Record& a, const Record& b) {
                return ascending ? (a.getDate() < b.getDate()) : (a.getDate() > b.getDate());
            });
        break;
    case SortField::CATEGORY:
        std::sort(records.begin(), records.end(),
            [ascending](const Record& a, const Record& b) {
                return ascending ? (a.getCategory() < b.getCategory()) : (a.getCategory() > b.getCategory());
            });
        break;
    case SortField::AMOUNT:
        std::sort(records.begin(), records.end(),
            [ascending](const Record& a, const Record& b) {
                return ascending ? (a.getAmount() < b.getAmount()) : (a.getAmount() > b.getAmount());
            });
        break;
    case SortField::USER:
        std::sort(records.begin(), records.end(),
            [ascending](const Record& a, const Record& b) {
                std::string userA = a.getUser() ? a.getUser()->getUsername() : "";
                std::string userB = b.getUser() ? b.getUser()->getUsername() : "";
                return ascending ? (userA < userB) : (userA > userB);
            });
        break;
    default:
        return false;
    }

    std::ofstream outputFile;
    if (!BinaryStorage<Record>::openDataFileForWrite(outputFile, std::ios::trunc, getDataFilename(familyName))) return false;
    return BinaryStorage<Record>::OverwriteAllToFile(outputFile, records);
}

void Family::FindByDate(const std::string& date) const {
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

    bool found = false;
    Record record;
    while (inputFile >> record) {
        if (record.getDate() == date) {
            std::cout << record << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "Записей с такой датой не найдено.\n";
    inputFile.close();
}

void Family::FindByCategory(const std::string& category) const {
    std::ifstream inputFile;
    if (!BinaryStorage<Record>::openDataFileForRead(inputFile, getDataFilename(familyName))) return;

    bool found = false;
    Record record;
    while (inputFile >> record) {
        if (record.getCategory() == category) {
            std::cout << record << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "Записей с такой категорией не найдено.\n";
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

    if (!found) {
        std::cout << "У вас пока нет записей о расходах.\n";
    }
    else {
        std::cout << "Общая сумма ваших расходов: " << total << "\n";
    }
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
    return out;
}