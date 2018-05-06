#include "return.h"

#include <iostream>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void returnBook(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the title of the book you wish to return: ";
    std::string title;
    std::getline(std::cin, title);

    //Find user and get array of borrowed books
    auto dbUser = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto array = dbUser->view()["borrowedBooks"].get_array().value;

    //Shows error but still works
    for(const bsoncxx::v_noabi::array::element &book : array) {
        auto dbBook = db[BOOKS].find_one(make_document(kvp("_id", book.get_value())));

        if(dbBook->view()["title"].get_utf8().value.to_string() == title) {
            db[BOOKS].update_one(
                make_document(kvp("_id", book.get_value())),
                make_document(kvp("$set", make_document(kvp("borrowedBy", ""))))
            );

            db[USERS].update_one(
                make_document(kvp("username", user.getUsername())),
                make_document(kvp("$pull", make_document(kvp("borrowedBooks", book.get_value()))))
            );

            std::cout << "Book " << title << " has been returned." << std::endl;

            return;
        }
    }

    //Check if book exists
    auto bookTest = db[BOOKS].find_one(make_document(kvp("title", title)));

    if(!bookTest) {
        std::cout << "The title you have entered does not exist." << std::endl;

    } else {
        std::cout << "You have not borrowed this book." << std::endl;
    }
}