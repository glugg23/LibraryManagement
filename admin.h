#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum AdminMenu {
    EXIT_A_M,
    CREATE_USER,
    MODIFY_USER,
    DELETE_USER,
    CREATE_BOOK,
    MODIFY_BOOK,
    DELETE_BOOK,
    VIEW_ALL_USERS,
    VIEW_ALL_BOOKS
};

void createUser(User &user, mongocxx::database &db);

void modifyUser(User &user, mongocxx::database &db);

void deleteUser(User &user, mongocxx::database &db);

void createBook(User &user, mongocxx::database &db);

void modifyBook(User &user, mongocxx::database &db);

void deleteBook(User &user, mongocxx::database &db);

void viewAllUsers(User &user, mongocxx::database &db);

void viewAllBooks(User &user, mongocxx::database &db);