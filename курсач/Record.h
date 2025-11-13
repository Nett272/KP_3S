#pragma once
#include <string>
#include <memory>
#include <fstream>
#include "User.h"

namespace FamilyBudget {

    class Record {
    private:
        std::shared_ptr<User> owner;
        int amount;
        std::string recordDate;
        std::string recordCategory;

    public:
        Record(std::shared_ptr<User> ownerPtr, int amount, const std::string& category, const std::string& date);
        Record();

        std::shared_ptr<User> GetUser() const { return owner; }
        int GetAmount() const { return amount; }
        const std::string& GetDate() const { return recordDate; }
        const std::string& GetCategory() const { return recordCategory; }

        void SetUser(std::shared_ptr<User> newUser) { owner = std::move(newUser); }
        void SetAmount(int newAmount) { amount = newAmount; }
        void SetDate(const std::string& newDate) { recordDate = newDate; }
        void SetCategory(const std::string& newCategory) { recordCategory = newCategory; }


        friend std::ofstream& operator<<(std::ofstream& out, const Record& record);
        friend std::ifstream& operator>>(std::ifstream& in, Record& record);
        friend std::ostream& operator<<(std::ostream& out, const Record& record);
        bool operator==(const Record& other) const;
    };

}