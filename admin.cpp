#include "admin.h"

#include <iostream>

#include <bsoncxx/json.hpp>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

void createUser(User &user, mongocxx::database &db) {
    std::cout << "Enter account username: ";
    std::string username;
    std::cin >> username;

    auto userResult = db[USERS].find_one(make_document(kvp("username", username)));

    if(!userResult) {
        char input;
        std::string role = "user";

        do {
            std::cout << "Would you like to make this account a admin user? (y/n): ";
            std::cin >> input;

            if(input == 'y' || input == 'Y') {
                role = "admin";
                break;
            }

        } while(!(input == 'n' || input == 'N'));

        auto result = db[USERS].insert_one(make_document(
            kvp("username", username),
            kvp("password", "password"),
            kvp("role", role),
            kvp("borrowedBooks", make_array())
        ));

        std::cout << "User " << username << " was created with the password 'password'." << std::endl;

    } else {
        std::cout << "This user already exists." << std::endl;
    }
}

void modifyUser(User &user, mongocxx::database &db) {
    std::cout << "modifyUser" << std::endl;
}

void deleteUser(User &user, mongocxx::database &db) {
    std::cout << "deleteUser" << std::endl;
}

void createBook(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter book title: ";
    std::string title;
    std::getline(std::cin, title);

    std::cout << "Enter book author: ";
    std::string author;
    std::getline(std::cin, author);

    std::cout << "Enter book genre: ";
    std::string genre;
    std::getline(std::cin, genre);

    auto result = db[BOOKS].insert_one(make_document(
        kvp("title", title),
        kvp("author", author),
        kvp("genre", genre),
        kvp("borrowedBy", "")
    ));

    std::cout << "Book " << title << " was created." << std::endl;
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
    //Find all books
    auto cursor = db[BOOKS].find({});

    //For all books
    for(const bsoncxx::document::view &doc : cursor) {
        //Find user who borrowed it
        auto result = db[USERS].find_one(make_document(kvp("_id", doc["borrowedBy"].get_value())));
        //Print data
        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        //If it was borrowed by a user, print that info
        if(result) {
            std::cout << ", borrowed by " << result->view()["username"].get_utf8().value << std::endl;

        } else {
            std::cout << std::endl;
        }
    }
}