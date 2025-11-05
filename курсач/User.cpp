#include "User.h"
#include <iostream>

User::User(std::string username, std::string password, std::string familyName, bool isAdmin)
    : username(username), familyName(familyName), isAdminFlag(isAdmin), passwordHash(HashPassword(password)) {
}

bool User::operator==(const User& other) const {
    return username == other.username &&
        passwordHash == other.passwordHash &&
        familyName == other.familyName &&
        isAdminFlag == other.isAdminFlag;
}

std::ofstream& operator<<(std::ofstream& out, const User& user) {
    size_t nameSize = user.username.size();
    out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    out.write(user.username.c_str(), nameSize);

    size_t hashSize = user.passwordHash.size();
    out.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
    out.write(user.passwordHash.c_str(), hashSize);

    size_t familySize = user.familyName.size();
    out.write(reinterpret_cast<const char*>(&familySize), sizeof(familySize));
    out.write(user.familyName.c_str(), familySize);

    out.write(reinterpret_cast<const char*>(&user.isAdminFlag), sizeof(user.isAdminFlag));
    return out;
}

std::ifstream& operator>>(std::ifstream& in, User& user) {
    size_t nameSize = 0;
    if (!in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize)) || nameSize > MAX_STRING_SIZE) {
        in.setstate(std::ios::failbit);
        return in;
    }
    user.username.resize(nameSize);
    if (!in.read(&user.username[0], nameSize)) return in;

    size_t hashSize = 0;
    if (!in.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize)) || hashSize > MAX_STRING_SIZE) {
        in.setstate(std::ios::failbit);
        return in;
    }
    user.passwordHash.resize(hashSize);
    if (!in.read(&user.passwordHash[0], hashSize)) return in;

    size_t familySize = 0;
    if (!in.read(reinterpret_cast<char*>(&familySize), sizeof(familySize)) || familySize > MAX_STRING_SIZE) {
        in.setstate(std::ios::failbit);
        return in;
    }
    user.familyName.resize(familySize);
    if (!in.read(&user.familyName[0], familySize)) return in;

    if (!in.read(reinterpret_cast<char*>(&user.isAdminFlag), sizeof(user.isAdminFlag))) return in;

    return in;
}

std::ostream& operator<<(std::ostream& out, const User& user) {
    out << "Имя пользователя: " << user.username << "\n";
    out << "Хэш пароля: " << user.passwordHash << "\n";
    out << "Название семьи: " << user.familyName << "\n";
    out << "Администратор: " << (user.isAdminFlag ? "Да" : "Нет") << "\n";
    return out;
}