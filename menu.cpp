#include "menu.h"

#include <iostream>

void loanMenu(User &user, mongocxx::database &db) {
    std::cout << "loanMenu" << std::endl;
}

void returnMenu(User &user, mongocxx::database &db) {
    std::cout << "returnMenu" << std::endl;
}

void adminMenu(User &user, mongocxx::database &db) {
    std::cout << "adminMenu" << std::endl;
}

void basicMenu(User &user, mongocxx::database &db) {
    std::cout << "Main Menu\n"
                 "0 - Loan a book\n"
                 "1 - Return a book\n"
                 "2 - Admin Options\n"
                 "3 - Exit and logout\n"
                 "Please enter your choice:\n" << std::endl;

    int choice;

    do {
        std::cout << user.getUsername() << ": ";
        std::cin >> choice;

        switch (choice) {
            case BOOK_LOAN:
                loanMenu(user, db);
                break;
            case BOOK_RETURN:
                returnMenu(user, db);
                break;
            case ADMIN_MENU:
                adminMenu(user, db);
                break;
            case EXIT:
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                std::cout << '\a';
                break;
        }

    } while(choice != 3);
}