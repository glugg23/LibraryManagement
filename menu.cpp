#include "menu.h"

#include <iostream>

#include "PRIVATE.h"
#include "loan.h"
#include "return.h"
#include "search.h"
#include "admin.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void loanMenu(User &user, mongocxx::database &db) {
    std::cout << "Loan Menu\n"
                 "\t1 - Take out a book\n"
                 "\t0 - Return to main menu\n"
                 "Please enter your choice:\n" << std::endl;

    //Get menu choice
    int choice;
    do {
        std::cout << user.getUsername() << ": ";
        std::cin >> choice;
        if(!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        switch(choice) {
            case LOAN_BOOK:
                loanBook(user, db);
                break;
            case EXIT_L_M:
                return;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
}

void returnMenu(User &user, mongocxx::database &db) {
    std::cout << "Return Menu\n"
                 "\t1 - Return a book\n"
                 "\t0 - Return to main menu\n"
                 "Please enter your choice:\n" << std::endl;

    //Find user and get all the books they have borrowed
    auto result = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto books = result->view()["borrowedBooks"].get_array().value;

    std::cout << "You have borrowed these books: " << std::endl;

    //Shows error but still works
    for(const bsoncxx::v_noabi::array::element &id : books) {
        //Finds book with current id and prints info
        auto book = db[BOOKS].find_one(make_document(kvp("_id", id.get_value())));
        auto date = book->view()["borrowedBy"]["date"].get_date().value;

        if(date < std::chrono::system_clock::now().time_since_epoch()) {
            std::cout << "OVERDUE: ";
        }

        std::cout << book->view()["title"].get_utf8().value
                  << " by " << book->view()["author"].get_utf8().value << std::endl;
    }
    std::cout << std::endl;

    //Get menu choice
    int choice;
    do {
        std::cout << user.getUsername() << ": ";
        std::cin >> choice;
        if(!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

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
    std::cout << "Search Menu\n"
                 "\t1 - Search by title\n"
                 "\t2 - Search by author\n"
                 "\t3 - Search by genre\n"
                 "\t4 - Advance search\n"
                 "\t0 - Return to main menu\n"
                 "Please enter your choice:\n" << std::endl;

    std::cout << "There are " << db[BOOKS].count({}) << " books in this library.\n" << std::endl;

    //Get menu choice
    int choice;
    do {
        std::cout << user.getUsername() << ": ";
        std::cin >> choice;
        if(!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        switch(choice) {
            case SEARCH_TITLE:
                searchBookTitle(user, db);
                break;
            case SEARCH_AUTHOR:
                searchBookAuthor(user, db);
                break;
            case SEARCH_GENRE:
                searchBookGenre(user, db);
                break;
            case SEARCH_ALL:
                searchBookAdvanced(user, db);
                break;
            case EXIT_S_M:
                return;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
}

void adminMenu(User &user, mongocxx::database &db) {
    //Checks to make sure user is an admin
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

        //Get menu choice
        int choice;
        do {
            std::cout << user.getUsername() << ": ";
            std::cin >> choice;
            if(!std::cin) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choice = -1;
            }

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
    //Check for overdue books
    auto result = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto books = result->view()["borrowedBooks"].get_array().value;
    int count = 0;

    //Shows error but still works
    for(const bsoncxx::v_noabi::array::element &id : books) {
        auto book = db[BOOKS].find_one(make_document(kvp("_id", id.get_value())));
        auto date = book->view()["borrowedBy"]["date"].get_date().value;

        if(date < std::chrono::system_clock::now().time_since_epoch()) {
            ++count;
        }
    }

    //Show overall count of overdue books
    if(count > 0) {
        if(count == 1) {
            std::cout << "You have 1 overdue book.\n" << std::endl;

        } else {
            std::cout << "You have " << count << " overdue books.\n" << std::endl;
        }
    }

    //Get menu choice
    int choice;
    do {
        std::cout << "Main Menu\n"
                     "\t1 - Loan a book\n"
                     "\t2 - Return a book\n"
                     "\t3 - Search for a book\n"
                     "\t4 - Admin Options\n"
                     "\t0 - Exit and logout\n"
                     "Please enter your choice:\n" << std::endl;

        std::cout << user.getUsername() << ": ";
        std::cin >> choice;
        if(!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

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
                return;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

    } while(choice != 0);
}