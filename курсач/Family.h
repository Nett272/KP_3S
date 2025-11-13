#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include "Record.h"
#include "BinaryStorage.h"
#include <functional>
#include <algorithm>
#include <iostream>

namespace FamilyBudget {

    class Family {
    private:
        std::string familyName;
        std::string passwordHash;

    public:
        Family(std::string familyName = "", std::string familyPassword = "");
        Family(const Family& other) = default;

        std::string GetFamilyName() const { return familyName; }
        std::string GetPasswordHash() const { return passwordHash; }
        static std::string GetDataFilename(const std::string& familyName);

        void SetFamilyName(const std::string& name) { familyName = name; }
        void SetFamilyPassword(const std::string& plainPassword) { passwordHash = HashPassword(plainPassword); }
        void SetFamilyPasswordHash(const std::string& hash) { passwordHash = hash; }



        // Основные операции
        bool RecordExpense(const Record& expenseRecord);
        void ShowFamilyExpenses();
        void ShowMyExpenses(const User& currentUser);
        bool SortRecords(std::function <bool(const Record&, const Record&)> comp);

        // Фильтры
        void FindRecord(std::function <bool(const Record&)> comp) const;
        void FilterByAmountRange(int minAmount, int maxAmount) const;
        void FilterByUserName(const std::string& username) const;
        void FilterByDateRange(const std::string& startDate, const std::string& endDate) const;

        // Отчёт
        void GenerateReport(const std::string& startDate, const std::string& endDate) const;

        // Сериализация
        friend std::ofstream& operator<<(std::ofstream& out, const Family& family);
        friend std::ifstream& operator>>(std::ifstream& in, Family& family);
        friend std::ostream& operator<<(std::ostream& out, const Family& family);
        bool operator==(const Family& other) const;
    };

}