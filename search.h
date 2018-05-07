#pragma once

#include <mongocxx/client.hpp>

#include "user.h"

enum SearchMenu {
    EXIT_S_M,
    SEARCH_TITLE,
    SEARCH_AUTHOR,
    SEARCH_GENRE,
    SEARCH_ALL
};

void searchBookTitle(User &user, mongocxx::database &db);

void searchBookAuthor(User &user, mongocxx::database &db);

void searchBookGenre(User &user, mongocxx::database &db);

void searchBookAdvanced(User &user, mongocxx::database &db);