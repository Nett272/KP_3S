#include "Menu.h"
#include <iostream>
#include <algorithm>
namespace FamilyBudget {

    bool Menu::UserAuthFlow(bool asAdmin) {
        while (true) {
            std::cout << "\n=== " << (asAdmin ? "Админская " : "") << "Аутентификация ===\n";
            int choice = ReadInt("1) Регистрация\n2) Авторизация\n0) Назад\nВыберите: ");

            if (choice == 1) {
                if (RegistrationFlow(asAdmin)) return true;
            }
            else if (choice == 2) {
                if (AuthorizationFlow(asAdmin)) return true;
            }
            else if (choice == 0) {
                return false;
            }
            else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
    }

    bool Menu::RegistrationFlow(bool asAdmin) {
        std::cout << "\n=== Регистрация ===\n";

        int familyChoice;
        while (true) {
            familyChoice = ReadInt("1) Создать новую семью\n2) Войти в существующую семью\n0) Назад\nВыберите: ");
            if (familyChoice == 0) return false;
            if (familyChoice == 1 || familyChoice == 2) break;
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }

        std::string familyName, familyPassword;

        if (familyChoice == 1) {
            while (true) {
                familyName = ReadLine("Название новой семьи: ");
                if (familyName.empty()) {
                    std::cout << "Название семьи не может быть пустым.\n";
                    continue;
                }

                familyPassword = ReadLine("Пароль семьи: ");
                if (familyPassword.empty()) {
                    std::cout << "Пароль не может быть пустым.\n";
                    continue;
                }

                Family newFamily(familyName, familyPassword);
                if (FamilyFileHandler.Registration(newFamily)) {
                    std::cout << "Семья '" << familyName << "' успешно создана!\n";
                    break;
                }
                else {
                    std::cout << "Семья с таким названием уже существует.\n";
                    int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в меню\nВыберите: ");
                    if (retryChoice == 2) return false;
                    else if (retryChoice != 1) {
                        std::cout << "Неверный выбор.\n";
                        return false;
                    }
                }
            }
        }
        else if (familyChoice == 2) {
            while (true) {
                familyName = ReadLine("Название семьи: ");
                familyPassword = ReadLine("Пароль семьи: ");

                Family familyProbe(familyName, familyPassword);
                if (FamilyFileHandler.Authorization(familyProbe)) {
                    std::cout << "Успешный вход в семью '" << familyName << "'\n";
                    break;
                }
                else {
                    std::cout << "Неверное название семьи или пароль.\n";
                    int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в меню\nВыберите: ");
                    if (retryChoice == 2) return false;
                    else if (retryChoice != 1) {
                        std::cout << "Неверный выбор.\n";
                        return false;
                    }
                }
            }
        }

        std::string username, password;
        while (true) {
            username = ReadLine("Имя пользователя: ");
            if (username.empty()) {
                std::cout << "Имя пользователя не может быть пустым.\n";
                continue;
            }

            password = ReadLine("Пароль: ");
            if (password.empty()) {
                std::cout << "Пароль не может быть пустым.\n";
                continue;
            }

            User newUser(username, password, familyName, asAdmin);
            if (UserFileHandler.Registration(newUser)) {
                CurrentUser = std::make_shared<User>(newUser);
                CurrentFamily = std::make_shared<Family>(familyName, familyPassword);
                std::cout << "Регистрация успешна! Добро пожаловать в семью '" << familyName << "'\n";
                return true;
            }
            else {
                std::cout << "Пользователь с таким именем уже существует.\n";
                int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в меню\nВыберите: ");
                if (retryChoice == 2) return false;
                else if (retryChoice != 1) {
                    std::cout << "Неверный выбор.\n";
                    return false;
                }
            }
        }
    }

    bool Menu::AuthorizationFlow(bool asAdmin) {
        std::cout << "\n=== Авторизация ===\n";

        User loadedUser("", "", "", asAdmin);
        while (true) {
            std::string username = ReadLine("Имя пользователя: ");
            std::string password = ReadLine("Пароль: ");

            loadedUser.SetUsername(username);
            loadedUser.SetPasswordHash(HashPassword(password));

            if (UserFileHandler.Authorization(loadedUser)) {
                break;
            }
            else {
                std::cout << "Неверное имя пользователя или пароль.\n";
                int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в меню\nВыберите: ");
                if (retryChoice == 2) return false;
                else if (retryChoice != 1) {
                    std::cout << "Неверный выбор.\n";
                    return false;
                }
            }
        }

        std::string userFamilyName = loadedUser.GetFamilyName();

        while (true) {
            std::string familyPassword = ReadLine("Пароль семьи '" + userFamilyName + "': ");

            Family familyProbe(userFamilyName, familyPassword);
            if (FamilyFileHandler.Authorization(familyProbe)) {
                CurrentUser = std::make_shared<User>(loadedUser);
                CurrentFamily = std::make_shared<Family>(familyProbe);
                std::cout << "Авторизация успешна! Добро пожаловать в семью '" << userFamilyName << "'\n";
                return true;
            }
            else {
                std::cout << "Неверный пароль семьи.\n";
                int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в меню\nВыберите: ");
                if (retryChoice == 2) return false;
                else if (retryChoice != 1) {
                    std::cout << "Неверный выбор.\n";
                    return false;
                }
            }
        }
    }

    void Menu::AuthenticatedMainMenu() {
        bool is_admin = CurrentUser->IsAdmin();

        while (true) {
            std::cout << "\n==============================================\n";
            std::cout << "            ГЛАВНОЕ МЕНЮ (" << CurrentUser->GetUsername() << (is_admin ? " — АДМИН" : "") << ")\n";
            std::cout << "==============================================\n";
            std::cout << "  1) Добавить расход/доход\n";
            std::cout << "  2) Редактировать расход\n";
            std::cout << "  3) Удалить расход\n";
            std::cout << "----------------------------------------------\n";
            std::cout << "  4) Сортировка расходов\n";
            std::cout << "  5) Поиск расходов\n";
            std::cout << "  6) Фильтрация расходов\n";
            std::cout << "----------------------------------------------\n";
            std::cout << "  7) Показать все расходы семьи\n";
            std::cout << "  8) Показать все мои расходы\n";
            std::cout << "  9) Показать мои данные\n";
            std::cout << "  10) Сформировать отчёт\n";
            std::cout << "  0) Выйти из аккаунта\n";

            if (is_admin) {
                std::cout << "----------------------------------------------\n";
                std::cout << " [Админские функции]\n";
                std::cout << " 11) Очистить файлы\n";
                std::cout << " 12) Просмотр файлов\n";
                std::cout << " 13) Удалить запись\n";
            }
            std::cout << "==============================================\n";

            int menu_choice = ReadInt("Выберите действие: ");

            if (menu_choice >= 1 && menu_choice <= 10) {
                switch (menu_choice) {
                case 1: {
                    int amount = ReadInt("Введите сумму: ");
                    std::string category = ReadLine("Введите категорию: ");
                    std::string date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (date.empty()) date = CurrentDate();
                    else InputDate(date);

                    Record new_record(CurrentUser, amount, category, date);
                    if (CurrentFamily->RecordExpense(new_record))
                        std::cout << "Запись успешно добавлена.\n";
                    else
                        std::cout << "Ошибка при добавлении записи.\n";
                    break;
                }
                case 2: {
                    std::cout << "\n=== Редактирование записи ===\n";
                    int old_amount = ReadInt("Введите сумму записи для изменения: ");
                    std::string old_category = ReadLine("Введите категорию: ");
                    std::string old_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (old_date.empty()) old_date = CurrentDate();
                    else InputDate(old_date);

                    Record old_record(CurrentUser, old_amount, old_category, old_date);

                    std::cout << "\nВведите новые данные:\n";
                    int new_amount = ReadInt("Новая сумма: ");
                    std::string new_category = ReadLine("Новая категория: ");
                    std::string new_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (new_date.empty()) new_date = CurrentDate();
                    else InputDate(new_date);

                    Record updated_record(CurrentUser, new_amount, new_category, new_date);

                    if (BinaryStorage<Record>::EditRecord(CurrentFamily->GetDataFilename(CurrentFamily->GetFamilyName()), old_record, updated_record))
                        std::cout << "Запись успешно отредактирована.\n";
                    else
                        std::cout << "Ошибка: запись не найдена или не принадлежит вам.\n";
                    break;
                }
                case 3: {
                    std::cout << "\n=== Удаление записи ===\n";
                    int delete_amount = ReadInt("Введите сумму записи для удаления: ");
                    std::string delete_category = ReadLine("Введите категорию: ");
                    std::string delete_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (delete_date.empty()) delete_date = CurrentDate();
                    else InputDate(delete_date);

                    Record target_record(CurrentUser, delete_amount, delete_category, delete_date);

                    if (BinaryStorage<Record>::RemoveRecord(CurrentFamily->GetDataFilename(CurrentFamily->GetFamilyName()), target_record))
                        std::cout << "Запись успешно удалена.\n";
                    else
                        std::cout << "Запись не найдена или принадлежит другому пользователю.\n";
                    break;
                }
                case 4: {
                    std::cout << "\n=== Сортировка расходов ===\n";
                    std::cout << "1) По дате (по возрастанию)\n2) По дате (по убыванию)\n";
                    std::cout << "3) По категории (по возрастанию)\n4) По категории (по убыванию)\n";
                    std::cout << "5) По сумме (по возрастанию)\n6) По сумме (по убыванию)\n";

                    int sort_choice = ReadInt("Выберите вариант сортировки: ");
                    bool sort_success = false;

                    switch (sort_choice) {
                    case 1: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetDate() > b.GetDate(); }); break;
                    case 2: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetDate() < b.GetDate(); }); break;
                    case 3: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetCategory() > b.GetCategory(); }); break;
                    case 4: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetCategory() < b.GetCategory(); }); break;
                    case 5: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetAmount() > b.GetAmount(); }); break;
                    case 6: sort_success = CurrentFamily->SortRecords([](const Record& a, const Record& b) {return a.GetAmount() < b.GetAmount(); }); break;
                    default: std::cout << "Неверный выбор.\n"; break;
                    }

                    std::cout << (sort_success ? "Сортировка успешно выполнена.\n" : "Ошибка сортировки.\n");
                    break;
                }
                case 5: {
                    std::cout << "\n=== Поиск расходов ===\n";
                    std::cout << "1) По дате\n2) По категории\n";
                    int search_choice = ReadInt("Выберите тип поиска: ");

                    if (search_choice == 1) {
                        std::string search_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                        if (search_date.empty()) search_date = CurrentDate();
                        else InputDate(search_date);
                        CurrentFamily->FindRecord([search_date](const Record& a) {return a.GetDate() == search_date; });
                    }
                    else if (search_choice == 2) {
                        std::string search_category = ReadLine("Введите категорию: ");
                        CurrentFamily->FindRecord([search_category](const Record& a) {return a.GetCategory() == search_category; });
                    }
                    else {
                        std::cout << "Неверный выбор.\n";
                    }
                    break;
                }
                case 6: {
                    std::cout << "\n=== Фильтрация расходов ===\n";
                    std::cout << "1) По диапазону сумм\n2) По имени пользователя\n3) По диапазону дат\n";
                    int filter_choice = ReadInt("Выберите тип фильтрации: ");

                    switch (filter_choice) {
                    case 1: {
                        int min_amount = ReadInt("Минимальная сумма: ");
                        int max_amount = ReadInt("Максимальная сумма: ");
                        CurrentFamily->FilterByAmountRange(min_amount, max_amount);
                        break;
                    }
                    case 2: {
                        std::string username = ReadLine("Введите имя пользователя: ");
                        CurrentFamily->FilterByUserName(username);
                        break;
                    }
                    case 3: {
                        std::string start_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                        if (start_date.empty()) start_date = CurrentDate();
                        else InputDate(start_date);
                        std::string end_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                        if (end_date.empty()) end_date = CurrentDate();
                        else InputDate(end_date);
                        CurrentFamily->FilterByDateRange(start_date, end_date);
                        break;
                    }
                    default:
                        std::cout << "Неверный выбор.\n";
                        break;
                    }
                    break;
                }
                case 7:
                    CurrentFamily->ShowFamilyExpenses();
                    break;
                case 8:
                    CurrentFamily->ShowMyExpenses(*CurrentUser);
                    break;
                case 9:
                    UserFileHandler.PrintInfo(*CurrentUser);
                    break;
                case 10: {
                    std::string start_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (start_date.empty()) start_date = CurrentDate();
                    else InputDate(start_date);
                    std::string end_date = ReadLine("Введите дату (YYYY-MM-DD, Enter — текущая): ");
                    if (end_date.empty()) end_date = CurrentDate();
                    else InputDate(end_date);
                    CurrentFamily->GenerateReport(start_date, end_date);
                    break;
                }
                }
            }
            else if (menu_choice == 0) {
                std::cout << "Выход из аккаунта.\n";
                break;
            }
            else if (is_admin && menu_choice >= 11 && menu_choice <= 13) {
                switch (menu_choice) {
                case 11: {
                    std::cout << "\n=== Очистка файлов ===\n";
                    std::cout << "1) Файл семей\n2) Файл пользователей\n3) Файл трат семьи\n";
                    int clear_choice = ReadInt("Выберите вариант: ");
                    switch (clear_choice) {
                    case 1: FamilyFileHandler.ClearFile(); break;
                    case 2: UserFileHandler.ClearFile(); break;
                    case 3: {
                        std::string familyName = ReadLine("Введите имя семьи файл трат которой хотите удалить: ");
                        FamilyFileHandler.ClearFamilyFile(familyName);
                        break;
                    }
                    default: std::cout << "Неверный выбор.\n"; break;
                    }
                    break;
                }
                case 12: {
                    std::cout << "\n=== Просмотреть файл ===\n";
                    std::cout << "1) Файл семей\n2) Файл пользователей\n3) Файл трат семьи\n";
                    int view_choice = ReadInt("Выберите вариант: ");
                    switch (view_choice) {
                    case 1: FamilyFileHandler.PrintFile(); break;
                    case 2: UserFileHandler.PrintFile(); break;
                    case 3: {
                        std::string familyName = ReadLine("Введите имя семьи файл трат которой хотите просмотреть: ");
                        FamilyFileHandler.PrintFamilyFile(familyName);
                        break;
                    }
                    default: std::cout << "Неверный выбор.\n"; break;
                    }
                    break;
                }
                case 13: {
                    std::cout << "\n=== Удалить запись ===\n";
                    std::cout << "1) Удалить семью\n2) Удалить пользователя\n";
                    int delete_choice = ReadInt("Выберите вариант: ");
                    switch (delete_choice) {
                    case 1: {
                        std::string familyName = ReadLine("Введите имя семьи которую хотите удалить: ");

                        Family targetFamily(familyName, "");
                        if (BinaryStorage<Family>::RemoveRecord(FamilyFileHandler.GetFilename(), targetFamily))
                            std::cout << "Семья успешно удалена.\n";
                        else
                            std::cout << "Семья не найдена.\n";
                        break;
                    }
                    case 2: {
                        std::string userName = ReadLine("Введите имя пользователя которого хотите удалить: ");

                        User targetUser(userName, "", "");
                        if (BinaryStorage<User>::RemoveRecord(UserFileHandler.GetFilename(), targetUser))
                            std::cout << "Пользователь успешно удален.\n";
                        else
                            std::cout << "Пользователь не найден.\n";
                        break;
                    }
                    default: std::cout << "Неверный выбор.\n"; break;
                    }
                    break;
                }
                }
            }
            else {
                std::cout << "Неверный выбор.\n";
            }

            system("pause");
            system("cls");
        }
    }



    void Menu::RunUserEntry() {
        if (UserAuthFlow(false)) {
            AuthenticatedMainMenu();
        }
    }

    void Menu::RunAdminEntry() {
        std::ifstream file("secret.txt", std::ios::in);
        if (!file.is_open()) {
            std::cout << "Ошибка открытия файла с паролем";
            return;
        }
        std::string real_password;
        file >> real_password;
        while (true) {
            std::string password = ReadLine("Введите админский пароль: ");
            if (password == real_password) {
                std::cout << "Пароль подтверждён. Продолжаем.\n";
                break;
            }
            else {
                std::cout << "Неверный админский пароль.\n";
                int retryChoice = ReadInt("1) Попробовать снова\n2) Вернуться в главное меню\nВыберите: ");
                if (retryChoice == 2) return;
            }
        }

        if (UserAuthFlow(true)) {
            AuthenticatedMainMenu();
        }
    }
}