#pragma once
#include <memory>
#include "User.h"
#include "Family.h"
#include "FileHandler.h"
#include "Utils.h"

namespace FamilyBudget {

    class Menu {
    private:
        UserFileHandler UserFileHandler;
        FamilyFileHandler FamilyFileHandler;
        std::shared_ptr<Family> CurrentFamily;
        std::shared_ptr<User> CurrentUser;

        bool UserAuthFlow(bool asAdmin);
        bool RegistrationFlow(bool asAdmin);
        bool AuthorizationFlow(bool asAdmin);
        void AuthenticatedMainMenu();

    public:
        void RunUserEntry();
        void RunAdminEntry();
    };

}