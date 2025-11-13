#pragma once
#include <string>
#include <memory>
#include "User.h"
#include "Family.h"
#include <iostream>
#include <algorithm>
#include "BinaryStorage.h"

namespace FamilyBudget {


    class FileHandler {
    public:
        virtual ~FileHandler() = default;
        virtual void PrintInfo(const User& user) = 0;
        virtual void PrintFile() = 0;
        virtual void ClearFile() = 0;
        virtual const std::string& GetFilename() const = 0;
    };


    class UserFileHandler : public FileHandler {
    private:
        static std::string filename;

    public:
        static void SetFilename(const std::string& fname) { filename = fname; }
        const std::string& GetFilename() const override { return filename; }

        bool Registration(const User& user);
        bool Authorization(User& user);
        void PrintInfo(const User& user) override;
        void PrintFile() override;
        void ClearFile() override;
    };


    class FamilyFileHandler : public FileHandler {
    private:
        static std::string filename;
        void DeleteAllFamilies();

    public:
        static void SetFilename(const std::string& fname) { filename = fname; }
        const std::string& GetFilename() const override { return filename; }

        bool Registration(const Family& family);
        bool Authorization(Family& family);
        void PrintInfo(const User& user) override;
        void PrintFile() override;
        void ClearFile() override;
        void PrintFamilyFile(const std::string& familyName);
        void ClearFamilyFile(std::string& familyName);
    };

}