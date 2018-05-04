#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum Menu {
    EXIT,
    BOOK_LOAN,
    BOOK_RETURN,
    BOOK_SEARCH,
    ADMIN_MENU,
};

void loanMenu(User &user, mongocxx::database &db);

void returnMenu(User &user, mongocxx::database &db);

void adminMenu(User &user, mongocxx::database &db);

void searchMenu(User &user, mongocxx::database &db);

void basicMenu(User &user, mongocxx::database &db);