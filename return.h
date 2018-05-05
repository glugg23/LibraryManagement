#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum ReturnMenu {
    EXIT_R_M,
    RETURN_BOOK
};

void returnBook(User &user, mongocxx::database &db);