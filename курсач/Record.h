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

        std::shared_ptr<User> getUser() const { return owner; }
        int getAmount() const { return amount; }
        const std::string& getDate() const { return recordDate; }
        const std::string& getCategory() const { return recordCategory; }

        void setUser(std::shared_ptr<User> newUser) { owner = std::move(newUser); }
        void setAmount(int newAmount) { amount = newAmount; }
        void setDate(const std::string& newDate) { recordDate = newDate; }
        void setCategory(const std::string& newCategory) { recordCategory = newCategory; }

        bool operator==(const Record& other) const;

        friend std::ofstream& operator<<(std::ofstream& out, const Record& record);
        friend std::ifstream& operator>>(std::ifstream& in, Record& record);
        friend std::ostream& operator<<(std::ostream& out, const Record& record);
    };

}