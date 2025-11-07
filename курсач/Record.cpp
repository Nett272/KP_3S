#include "Record.h"
#include <iostream>

namespace FamilyBudget {

    Record::Record(std::shared_ptr<User> ownerPtr, int amount, const std::string& category, const std::string& date)
        : owner(std::move(ownerPtr)), amount(amount), recordDate(date), recordCategory(category) {
    }

    Record::Record() : owner(nullptr), amount(0), recordDate(""), recordCategory("") {}

    bool Record::operator==(const Record& other) const {
        return (owner && other.owner && owner->getUsername() == other.owner->getUsername() &&
            amount == other.amount &&
            recordDate == other.recordDate &&
            recordCategory == other.recordCategory);
    }

    std::ofstream& operator<<(std::ofstream& out, const Record& record) {
        std::string name = record.owner ? record.owner->getUsername() : "";
        size_t nameLength = name.size();
        out.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        out.write(name.c_str(), nameLength);

        out.write(reinterpret_cast<const char*>(&record.amount), sizeof(record.amount));

        size_t categoryLength = record.recordCategory.size();
        out.write(reinterpret_cast<const char*>(&categoryLength), sizeof(categoryLength));
        out.write(record.recordCategory.c_str(), categoryLength);

        size_t dateLength = record.recordDate.size();
        out.write(reinterpret_cast<const char*>(&dateLength), sizeof(dateLength));
        out.write(record.recordDate.c_str(), dateLength);

        return out;
    }

    std::ifstream& operator>>(std::ifstream& in, Record& record) {
        size_t nameLength = 0;
        if (!in.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength)) || nameLength > 10000) {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string name;
        if (nameLength > 0) {
            name.resize(nameLength);
            if (!in.read(&name[0], nameLength)) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        if (!in.read(reinterpret_cast<char*>(&record.amount), sizeof(record.amount))) {
            in.setstate(std::ios::failbit);
            return in;
        }

        size_t categoryLength = 0;
        if (!in.read(reinterpret_cast<char*>(&categoryLength), sizeof(categoryLength)) || categoryLength > 10000) {
            in.setstate(std::ios::failbit);
            return in;
        }
        record.recordCategory.clear();
        if (categoryLength > 0) {
            record.recordCategory.resize(categoryLength);
            if (!in.read(&record.recordCategory[0], categoryLength)) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        size_t dateLength = 0;
        if (!in.read(reinterpret_cast<char*>(&dateLength), sizeof(dateLength)) || dateLength > 10000) {
            in.setstate(std::ios::failbit);
            return in;
        }
        record.recordDate.clear();
        if (dateLength > 0) {
            record.recordDate.resize(dateLength);
            if (!in.read(&record.recordDate[0], dateLength)) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        auto tempUser = std::make_shared<User>();
        tempUser->setUsername(name);
        record.setUser(tempUser);

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const Record& record) {
        out << "Имя: " << (record.owner ? record.owner->getUsername() : "Неизвестный") << "\n";
        out << "Сумма: " << record.amount << "\n";
        out << "Категория: " << record.recordCategory << "\n";
        out << "Дата: " << record.recordDate << "\n";
        return out;
    }

}