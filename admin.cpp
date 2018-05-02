#include "admin.h"

#include <iostream>

#include <bsoncxx/json.hpp>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

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
    //Find all users
    auto cursor = db[USERS].find({});

    //For all users in collection
    for(const bsoncxx::document::view &doc : cursor) {
        //Find array of all their borrowed books
        auto ids = doc["borrowedBooks"].get_array().value;

        //Print their username
        std::cout << "Username: " << doc["username"].get_utf8().value << std::endl;

        std::cout << "Loaned books:" << std::endl;

        //For all book ids in borrowed book array
        for(const bsoncxx::v_noabi::array::element &id : ids) {
            //Find corresponding book document
            auto result = db[BOOKS].find_one(make_document(kvp("_id", id.get_value())));

            //Print title and author
            std::cout << result->view()["title"].get_utf8().value
                      << " by " << result->view()["author"].get_utf8().value << std::endl;

            //std::cout << bsoncxx::to_json(*result) << std::endl;
        }
        //std::cout << bsoncxx::to_json(doc) << std::endl;
        std::cout << std::endl;
    }
}

void viewAllBooks(User &user, mongocxx::database &db) {
    std::cout << "viewAllBooks" << std::endl;
}