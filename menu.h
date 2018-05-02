#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum Menu {
    BOOK_LOAN,
    BOOK_RETURN,
    ADMIN_MENU,
    EXIT
};

void loanMenu(User &user, mongocxx::database &db);

void returnMenu(User &user, mongocxx::database &db);

void adminMenu(User &user, mongocxx::database &db);

void basicMenu(User &user, mongocxx::database &db);