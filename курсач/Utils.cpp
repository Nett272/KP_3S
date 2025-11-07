#include "Utils.h"

namespace FamilyBudget {

    std::string current_date() {
        time_t t = time(nullptr);
        tm tm{};
#if defined(_MSC_VER)
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
        return std::string(buf);
    }

    void clearInputStream() {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

    int ReadInt(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInputStream();
                return value;
            }
            else {
                std::cout << "Ошибка ввода! Введите корректное число.\n";
                clearInputStream();
            }
        }
    }

    std::string ReadLine(const std::string& prompt) {
        std::string line;
        while (true) {
            std::cout << prompt;
            if (!std::getline(std::cin, line)) {
                std::cout << "Ошибка ввода! Попробуйте снова.\n";
                clearInputStream();
                continue;
            }
            return line;
        }
    }

    std::string HashPassword(const std::string& password) {
        return std::to_string(std::hash<std::string>{}(password));
    }
}