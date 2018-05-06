#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum LoanMenu {
    EXIT_L_M,
    LOAN_BOOK
};

void loanBook(User &user, mongocxx::database &db);