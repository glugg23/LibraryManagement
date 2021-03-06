#include "admin.h"

#include <iostream>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

void createUser(User &user, mongocxx::database &db) {
    std::cout << "Enter account username: ";
    std::string username;
    std::cin >> username;

    std::cout << "Enter account password: ";
    std::string password;
    std::cin >> password;

    //Check is user exists already
    auto userResult = db[USERS].find_one(make_document(kvp("username", username)));

    if(!userResult) {
        char input;
        std::string role = "user";

        std::cout << "Would you like to make this account an admin user? (y/n): ";
        std::cin >> input;

        if(input == 'y') {
            role = "admin";
        }

        auto result = db[USERS].insert_one(make_document(
            kvp("username", username),
            kvp("password", password),
            kvp("role", role),
            kvp("borrowedBooks", make_array()))
        );

        if(role == "admin") {
            std::cout << "Admin " << username << " was created with the password " << password << "." << std::endl;

        } else {
            std::cout << "User " << username << " was created with the password " << password << "." << std::endl;
        }

    } else {
        std::cout << "This user already exists." << std::endl;
    }

    std::cout << std::endl;
}

void modifyUser(User &user, mongocxx::database &db) {
    std::cout << "Enter user to modify: ";
    std::string username;
    std::cin >> username;

    auto result = db[USERS].find_one(make_document(kvp("username", username)));

    if(result) {
        char input;
        //Set these to what they already are so we can do easier updates
        std::string newUsername = username;
        std::string password = result->view()["password"].get_utf8().value.to_string();

        std::cout << "Change username? (y/n): ";
        std::cin >> input;

        if(input == 'y') {
            std::cout << "Enter new username: ";
            std::cin >> newUsername;
        }

        std::cout << "Change password? (y/n): ";
        std::cin >> input;

        if(input == 'y') {
            std::cout << "Enter new password: ";
            std::cin >> password;
        }

        db[USERS].update_one(
            make_document(kvp("username", username)),
            make_document(kvp("$set", make_document(
                kvp("username", newUsername),
                kvp("password", password)
            )))
        );

        std::cout << "User " << username << " has been modified." << std::endl;

    } else {
        std::cout << "User " << username << " does not exist." << std::endl;
    }

    std::cout << std::endl;
}

void deleteUser(User &user, mongocxx::database &db) {
    std::cout << "Enter user to delete: ";
    std::string username;
    std::cin >> username;

    if(username == user.getUsername()) {
        //No clue how to handle this, so I just won't allow it
        std::cout << "Not a good idea to delete your own account..." << std::endl;
        return;
    }

    auto result = db[USERS].find_one(make_document(kvp("username", username)));

    if(result) {
        auto ids = result->view()["borrowedBooks"].get_array().value;

        //Shows error but still works
        for(const bsoncxx::v_noabi::array::element &id : ids) {
            db[BOOKS].update_one(
                make_document(kvp("_id", id.get_value())),
                make_document(kvp("$set", make_document(kvp("borrowedBy",
                    make_document(
                        kvp("user", bsoncxx::types::b_null()),
                        kvp("date", bsoncxx::types::b_null()),
                        kvp("isBorrowed", false))))))
            );
        }

        db[USERS].delete_one(make_document(kvp("username", username)));
        std::cout << "User " << username << " was deleted." << std::endl;

    } else {
        std::cout << "User " << username << " does not exist." << std::endl;
    }

    std::cout << std::endl;
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
        kvp("borrowedBy", make_document(
            kvp("user", bsoncxx::types::b_null()),
            kvp("date", bsoncxx::types::b_null()),
            kvp("isBorrowed", false)
        ))
    ));

    std::cout << "Book " << title << " was created." << std::endl;

    std::cout << std::endl;
}

void modifyBook(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter book to modify: ";
    std::string title;
    std::getline(std::cin, title);

    auto result = db[BOOKS].find_one(make_document(kvp("title", title)));

    if(result) {
        //Set these to what they already are so we can do easier updates
        std::string input;
        std::string newTitle = title;
        std::string author = result->view()["author"].get_utf8().value.to_string();
        std::string genre = result->view()["genre"].get_utf8().value.to_string();

        std::cout << "Change title? (y/n): ";
        std::getline(std::cin, input);

        if(input == "y") {
            std::cout << "Enter new title: ";
            std::getline(std::cin, newTitle);
        }

        std::cout << "Change author? (y/n): ";
        std::getline(std::cin, input);

        if(input == "y") {
            std::cout << "Enter new author: ";
            std::getline(std::cin, author);
        }

        std::cout << "Change genre? (y/n): ";
        std::getline(std::cin, input);

        if(input == "y") {
            std::cout << "Enter new genre: ";
            std::getline(std::cin, genre);
        }

        db[BOOKS].update_one(
            make_document(kvp("title", title)),
            make_document(kvp("$set", make_document(
                kvp("title", newTitle),
                kvp("author", author),
                kvp("genre", genre)
            )))
        );

        std::cout << "Book " << title << " has been modified." << std::endl;

    } else {
        std::cout << "Book " << title << " does not exist." << std::endl;
    }

    std::cout << std::endl;
}

void deleteBook(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter title of book you wish to delete: ";
    std::string title;
    std::getline(std::cin, title);

    auto result = db[BOOKS].find_one(make_document(kvp("title", title)));

    if(result) {
        //Find id of who borrowed the book
        auto element = result->view()["borrowedBy"]["user"].get_value();

        //Remove that book from their borrowed list
        db[USERS].update_one(
            make_document(kvp("_id", element)),
            make_document(kvp("$pull", make_document(kvp("borrowedBooks", result->view()["_id"].get_oid()))))
        );

        //Then delete the book itself
        db[BOOKS].delete_one(make_document(kvp("title", title)));
        std::cout << "Book " << title << " was deleted." << std::endl;

    } else {
        std::cout << "Book " << title << " does not exist." << std::endl;
    }

    std::cout << std::endl;
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
        //Shows error but still works
        for(const bsoncxx::v_noabi::array::element &id : ids) {
            //Find corresponding book document
            auto result = db[BOOKS].find_one(make_document(kvp("_id", id.get_value())));

            //Print title and author
            std::cout << result->view()["title"].get_utf8().value
                      << " by " << result->view()["author"].get_utf8().value << std::endl;
        }

        std::cout << std::endl;
    }
}

void viewAllBooks(User &user, mongocxx::database &db) {
    //Find all books
    auto cursor = db[BOOKS].find({});

    //For all books
    //Shows error but still works
    for(const bsoncxx::document::view &doc : cursor) {
        //Find user who borrowed it
        auto result = db[USERS].find_one(make_document(kvp("_id", doc["borrowedBy"]["user"].get_value())));
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

    std::cout << std::endl;
}