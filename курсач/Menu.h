#pragma once
#include <memory>
#include "User.h"
#include "Family.h"
#include "FileHandler.h"
#include "Utils.h"

namespace FamilyBudget {

    class Menu {
    private:
        UserFileHandler user_file_handler;
        FamilyFileHandler family_file_handler;
        std::shared_ptr<Family> current_family;
        std::shared_ptr<User> current_user;
        const int SCAM = 1;

        bool UserAuthFlow(bool asAdmin);
        bool RegistrationFlow(bool asAdmin);
        bool AuthorizationFlow(bool asAdmin);
        void AuthenticatedMainMenu();

    public:
        void RunUserEntry();
        void RunAdminEntry();
    };

}