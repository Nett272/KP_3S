#pragma once
#include <string>
#include <memory>
#include <fstream>
#include "Utils.h"

#define MAX_STRING_SIZE 10000

namespace FamilyBudget {

    class User {
    private:
        std::string username;
        std::string passwordHash;
        std::string familyName;
        bool isAdminFlag;

    public:
        User(std::string username = "", std::string password = "", std::string familyName = "", bool isAdmin = false);
        User(const User& other) = default;
        User& operator=(const User& other) = default;

        bool operator==(const User& other) const;

        const std::string& GetUsername() const { return username; }
        const std::string& GetPasswordHash() const { return passwordHash; }
        const std::string& GetFamilyName() const { return familyName; }
        bool IsAdmin() const { return isAdminFlag; }

        void SetUsername(const std::string& username) { this->username = username; }
        void SetFamilyName(const std::string& familyName) { this->familyName = familyName; }
        void SetPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }

        friend std::ofstream& operator<<(std::ofstream& out, const User& user);
        friend std::ifstream& operator>>(std::ifstream& in, User& user);
        friend std::ostream& operator<<(std::ostream& out, const User& user);
    };

}