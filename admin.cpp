#include "admin.h"

#include <iostream>

void createUser(User &user, mongocxx::database &db) {
    std::cout << "createUser" << std::endl;
}

void modifyUser(User &user, mongocxx::database &db) {
    std::cout << "modifyUser" << std::endl;
}

void deleteUser(User &user, mongocxx::database &db) {
    std::cout << "deleteUser" << std::endl;
}

void createBook(User &user, mongocxx::database &db) {
    std::cout << "createBook" << std::endl;
}

void modifyBook(User &user, mongocxx::database &db) {
    std::cout << "modifyBook" << std::endl;
}

void deleteBook(User &user, mongocxx::database &db) {
    std::cout << "deleteBook" << std::endl;
}

void viewAllUsers(User &user, mongocxx::database &db) {
    std::cout << "viewAllUsers" << std::endl;
}

void viewAllBooks(User &user, mongocxx::database &db) {
    std::cout << "viewAllBooks" << std::endl;
}