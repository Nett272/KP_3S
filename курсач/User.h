#pragma once
#include <string>
#include <memory>
#include <fstream>
#include "Utils.h"

#define MAX_STRING_SIZE 10000

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

    const std::string& getUsername() const { return username; }
    const std::string& getPasswordHash() const { return passwordHash; }
    const std::string& getFamilyName() const { return familyName; }
    bool isAdmin() const { return isAdminFlag; }

    void setUsername(const std::string& u) { username = u; }
    void setFamilyName(const std::string& f) { familyName = f; }
    void setPasswordHash(const std::string& hash) { passwordHash = hash; }

    friend std::ofstream& operator<<(std::ofstream& out, const User& user);
    friend std::ifstream& operator>>(std::ifstream& in, User& user);
    friend std::ostream& operator<<(std::ostream& out, const User& user);
};