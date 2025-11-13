#include "Utils.h"

namespace FamilyBudget {

    std::string CurrentDate() {
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

    void ClearInputStream() {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

    int ReadInt(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                ClearInputStream();
                return value;
            }
            else {
                std::cout << "Ошибка ввода! Введите корректное число.\n";
                ClearInputStream();
            }
        }
    }

    std::string ReadLine(const std::string& prompt) {
        std::string line;
        while (true) {
            std::cout << prompt;
            if (!std::getline(std::cin, line)) {
                std::cout << "Ошибка ввода. Попробуйте снова.\n";
                ClearInputStream();
                continue;
            }
            return line;
        }
    }

    std::string HashPassword(const std::string& password) {
        return std::to_string(std::hash<std::string>{}(password));
    }

    void InputDate(std::string& date) {
        std::string current = CurrentDate();
        bool isMistake = false;

        while (true) {
            isMistake = false;

            if (date.length() == 10) {

                if (date[4] != '-' || date[7] != '-') {
                    std::cout << "Неверный формат! Используйте формат ГГГГ-ММ-ДД.\n";
                    isMistake = true;
                }

                bool valid = true;
                for (int i = 0; i < 10; i++) {
                    if (i != 4 && i != 7 && !isdigit(date[i])) {
                        std::cout << "Дата должна содержать только цифры и дефисы!\n";
                        isMistake = true;
                        break;
                    }
                }

                int year = std::stoi(date.substr(0, 4));
                int month = std::stoi(date.substr(5, 2));
                int day = std::stoi(date.substr(8, 2));

                if (month < 1 || month > 12) {
                    std::cout << "Месяц должен быть от 1 до 12!\n";
                    isMistake = true;
                }


                int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

                if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
                    days_in_month[1] = 29;
                }

                if (day < 1 || day > days_in_month[month - 1]) {
                    std::cout << "Несуществующий день для этого месяца.\n";
                    isMistake = true;
                }


                if (date > current) {
                    std::cout << "Дата не может быть больше текущей. Текущая дата: " << current << "\n";
                    isMistake = true;
                }
            }
            else {
                std::cout << "Неверный формат даты! Должно быть 10 символов.\n";
                isMistake = true;
            }


            if (!isMistake) return;
            else {
                while (true) {
                    int retryChoice = ReadInt("1) Попробовать снова\n2) Ввести текущую дату\nВыберите: ");
                    if (retryChoice == 1) {
                        date = ReadLine("Введите дату в формате ГГГГ-ММ-ДД: ");
                        break;
                    }
                    else if (retryChoice == 2) {
                        date = current;
                        return;
                    }
                    else std::cout << "Неверный выбор.\n";
                }
            }
        }
    }
}