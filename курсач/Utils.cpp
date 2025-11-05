#include "Utils.h"
#include <windows.h>
#include <sstream>
#include <iomanip>
using namespace FamilyBudget;

using namespace std;

string current_date() {
    time_t t = time(nullptr);
    tm tm{};
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
    return string(buf);
}

void clearInputStream() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin.sync();
}

int ReadInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputStream();
            return value;
        }
        else {
            cout << "Ошибка ввода! Введите корректное число.\n";
            clearInputStream();
        }
    }
}

string ReadLine(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            cout << "Ошибка ввода! Попробуйте снова.\n";
            clearInputStream();
            continue;
        }
        return line;
    }
}

string HashPassword(const string& password) {
    return to_string(hash<string>{}(password));
}