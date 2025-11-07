#include "Menu.h"
#include <Windows.h>
using namespace FamilyBudget;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Menu menu;
    bool running = true;
    int choice;

    while (running) {
        std::cout << "\n=== Главная ===\n";
        choice = ReadInt("1) Войти как пользователь\n2) Войти как админ\n0) Выход\nВыберите: ");

        switch (choice) {
        case 1: menu.RunUserEntry(); break;
        case 2: menu.RunAdminEntry(); break;
        case 0: std::cout << "Пока!\n"; running = false; break;
        default: std::cout << "Неверный выбор.\n"; break;
        }
        system("pause");
        system("cls");
    }

    return 0;
}