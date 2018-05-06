#include "menu.h"

#include <iostream>

#include "PRIVATE.h"
#include "return.h"
#include "admin.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void loanMenu(User &user, mongocxx::database &db) {
    std::cout << "loanMenu" << std::endl;
}

void returnMenu(User &user, mongocxx::database &db) {
    std::cout << "Return Menu\n"
                 "\t1 - Return a book\n"
                 "\t0 - Return to main menu\n"
                 "Please enter your choice:\n" << std::endl;

    auto result = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto books = result->view()["borrowedBooks"].get_array().value;

    std::cout << "You have borrowed these books: " << std::endl;

    for(const bsoncxx::v_noabi::array::element &id : books) {
        auto book = db[BOOKS].find_one(make_document(kvp("_id", id.get_value())));
        std::cout << book->view()["title"].get_utf8().value
                  << " by " << book->view()["author"].get_utf8().value << std::endl;
    }
    std::cout << std::endl;

    int choice;

    do {
        std::cout << user.getUsername() << ": ";
        std::cin >> choice;

        switch(choice) {
            case RETURN_BOOK:
                returnBook(user, db);
                break;
            case EXIT_R_M:
                return;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
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
        std::cout << "Admin Menu\n"
                     "\t1 - Create new user\n"
                     "\t2 - Modify user\n"
                     "\t3 - Delete user\n"
                     "\t4 - Create new book\n"
                     "\t5 - Modify book\n"
                     "\t6 - Delete book\n"
                     "\t7 - View all users\n"
                     "\t8 - View all books\n"
                     "\t0 - Return to main menu\n"
                     "Please enter your choice:\n" << std::endl;

        int choice;

        do {
            std::cout << user.getUsername() << ": ";
            std::cin >> choice;

            switch(choice) {
                case CREATE_USER:
                    createUser(user, db);
                    break;
                case MODIFY_USER:
                    modifyUser(user, db);
                    break;
                case DELETE_USER:
                    deleteUser(user, db);
                    break;
                case CREATE_BOOK:
                    createBook(user, db);
                    break;
                case MODIFY_BOOK:
                    modifyBook(user, db);
                    break;
                case DELETE_BOOK:
                    deleteBook(user, db);
                    break;
                case VIEW_ALL_USERS:
                    viewAllUsers(user, db);
                    break;
                case VIEW_ALL_BOOKS:
                    viewAllBooks(user, db);
                    break;
                case EXIT_A_M:
                    return;
                default:
                    std::cout << "Invalid option." << std::endl;
                    break;
            }

        } while(choice != 0);
    }
}

void basicMenu(User &user, mongocxx::database &db) {
    Start:
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
                goto Start;
            case BOOK_RETURN:
                returnMenu(user, db);
                goto Start;
            case BOOK_SEARCH:
                searchMenu(user, db);
                goto Start;
            case ADMIN_MENU:
                adminMenu(user, db);
                goto Start;
            case EXIT:
                return;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
}