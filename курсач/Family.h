#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include "Record.h"
#include "BinaryStorage.h"

enum class SortField {
    DATE, CATEGORY, AMOUNT, USER
};

class Family {
private:
    std::string familyName;
    std::string passwordHash;

public:
    static std::string getDataFilename(const std::string& familyName);

    Family(std::string familyName = "", std::string familyPassword = "");
    Family(const Family& other) = default;
    Family& operator=(const Family& other) = default;

    bool operator==(const Family& other) const;

    std::string getFamilyName() const { return familyName; }
    std::string getPasswordHash() const { return passwordHash; }

    void setFamilyName(const std::string& name) { familyName = name; }
    void setFamilyPassword(const std::string& plainPassword) { passwordHash = HashPassword(plainPassword); }
    void setFamilyPasswordHash(const std::string& hash) { passwordHash = hash; }

    // Основные операции
    bool recordExpense(const Record& expenseRecord);
    void printInfo();
    bool deleteRecord(const Record& targetRecord);
    bool editRecord(const Record& targetRecord, const Record& updatedRecord);
    bool sortRecords(SortField sortBy, bool ascending = true);

    // Фильтры
    void FindByDate(const std::string& date) const;
    void FindByCategory(const std::string& category) const;
    void FilterByAmountRange(int minAmount, int maxAmount) const;
    void FilterByUserName(const std::string& username) const;
    void FilterByDateRange(const std::string& startDate, const std::string& endDate) const;

    // Отчёт
    void generateReport(const std::string& startDate, const std::string& endDate) const;
    void ShowMyExpenses(const User& currentUser);

    // Сериализация
    friend std::ofstream& operator<<(std::ofstream& out, const Family& family);
    friend std::ifstream& operator>>(std::ifstream& in, Family& family);
    friend std::ostream& operator<<(std::ostream& out, const Family& family);
};