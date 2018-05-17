#include "loan.h"

#include <iostream>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void loanBook(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the title of the book you wish to borrow: ";
    std::string title;
    std::getline(std::cin, title);

    //Find current user and book that they wish to borrow
    auto dbUser = db[USERS].find_one(make_document(kvp("username", user.getUsername())));
    auto dbBook = db[BOOKS].find_one(make_document(kvp("title", title)));

    if(dbBook) {
        //Check value of this field to see if it's been borrowed or not
        //TODO Add date when book should be returned by
        if(!dbBook->view()["borrowedBy"]["isBorrowed"].get_value().get_bool()) {
            db[BOOKS].update_one(
                make_document(kvp("title", title)),
                make_document(kvp("$set", make_document(kvp("borrowedBy",
                make_document(
                    kvp("user", dbUser->view()["_id"].get_oid()),
                    kvp("date", bsoncxx::types::b_null()),
                    kvp("isBorrowed", true))))))
            );

            db[USERS].update_one(
                make_document(kvp("username", user.getUsername())),
                make_document(kvp("$push", make_document(kvp("borrowedBooks", dbBook->view()["_id"].get_oid()))))
            );

            std::cout << "You borrowed " << title << "." << std::endl;

        } else {
            std::cout << "This book was already loaned out." << std::endl;
        }

    } else {
        std::cout << "This book does not exist." << std::endl;
    }

    std::cout << std::endl;
}