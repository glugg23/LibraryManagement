#include "menu.h"

#include <iostream>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void loanMenu(User &user, mongocxx::database &db) {
    std::cout << "loanMenu" << std::endl;
}

void returnMenu(User &user, mongocxx::database &db) {
    std::cout << "returnMenu" << std::endl;
}

void searchMenu(User &user, mongocxx::database &db) {
    std::cout << "searchMenu" << std::endl;
}

void adminMenu(User &user, mongocxx::database &db) {
    auto document = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto element = document->view()["role"];

    if(element.get_utf8().value.to_string() != "admin") {
        std::cout << "You do not have admin privileges." << std::endl;
        return;

    } else {
        std::cout << "Admin Menu\n" << std::endl;
    }
}

void basicMenu(User &user, mongocxx::database &db) {
    std::cout << "Main Menu\n"
                 "\t1 - Loan a book\n"
                 "\t2 - Return a book\n"
                 "\t3 - Search for a book\n"
                 "\t4 - Admin Options\n"
                 "\t0 - Exit and logout\n"
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
            case BOOK_SEARCH:
                searchMenu(user, db);
                break;
            case ADMIN_MENU:
                adminMenu(user, db);
                break;
            case EXIT:
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
}