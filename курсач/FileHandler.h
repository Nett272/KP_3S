#pragma once
#include <string>
#include <memory>
#include "User.h"
#include "Family.h"
#include "BinaryStorage.h"

class FileHandler {
public:
    virtual ~FileHandler() = default;
    virtual void PrintInfo(const User& user) = 0;
    virtual void PrintFile() = 0;
    virtual void ClearFile() = 0;

protected:
    virtual const std::string& GetFilename() const = 0;
};

class UserFileHandler : public FileHandler {
private:
    static std::string filename;

protected:
    const std::string& GetFilename() const override { return filename; }

public:
    static void SetFilename(const std::string& fname) { filename = fname; }

    bool Registration(const User& user);
    bool Authorization(User& user);
    void PrintInfo(const User& user) override;
    void PrintFile() override;
    void ClearFile() override;
    bool DeleteUser(std::string& userName);
};

class FamilyFileHandler : public FileHandler {
private:
    static std::string filename;
    void DeleteAllFamilies();

protected:
    const std::string& GetFilename() const override { return filename; }

public:
    static void SetFilename(const std::string& fname) { filename = fname; }

    bool CreateFamily(const Family& family);
    bool AuthorizationInFamily(Family& family);
    void PrintInfo(const User& user) override;
    void PrintFile() override;
    void ClearFile() override;
    void PrintFamilyFile(const std::string& familyName);
    void ClearFamilyFile(std::string& familyName);
    bool DeleteFamily(std::string& familyName);
};