#include "search.h"

#include <iostream>

#include "PRIVATE.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void searchBookTitle(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the title of the book you wish to search for: ";
    std::string title;
    std::getline(std::cin, title);

    //Finds books where the title given is at least a part of it, not looking exclusively for exact matches
    auto cursor = db[BOOKS].find(make_document(kvp("title",
        make_document(kvp("$regex", title), kvp("$options", "i")))));

    std::cout << std::endl;

    //Shows error but still works
    for(const bsoncxx::document::view &doc : cursor) {
        //Check if the book has already been borrowed
        auto result = db[USERS].find_one(make_document(kvp("_id", doc["borrowedBy"].get_value())));

        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(result) {
            std::cout << ", currently unavailable" << std::endl;

        } else {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

void searchBookAuthor(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the author of the book you wish to search for: ";
    std::string author;
    std::getline(std::cin, author);

    //Finds books where the author name given is at least a part of it, not looking exclusively for exact matches
    auto cursor = db[BOOKS].find(make_document(kvp("author",
        make_document(kvp("$regex", author), kvp("$options", "i")))));

    std::cout << std::endl;

    //Shows error but still works
    for(const bsoncxx::document::view &doc : cursor) {
        //Check if the book has already been borrowed
        auto result = db[USERS].find_one(make_document(kvp("_id", doc["borrowedBy"].get_value())));

        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(result) {
            std::cout << ", currently unavailable" << std::endl;

        } else {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

void searchBookGenre(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the genre of the book you wish to search for: ";
    std::string genre;
    std::getline(std::cin, genre);

    //Finds books where the genre given is at least a part of it, not looking exclusively for exact matches
    auto cursor = db[BOOKS].find(make_document(kvp("genre",
        make_document(kvp("$regex", genre), kvp("$options", "i")))));

    std::cout << std::endl;

    //Shows error but still works
    for(const bsoncxx::document::view &doc : cursor) {
        //Check if the book has already been borrowed
        auto result = db[USERS].find_one(make_document(kvp("_id", doc["borrowedBy"].get_value())));

        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(result) {
            std::cout << ", currently unavailable" << std::endl;

        } else {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

void searchBookAdvanced(User &user, mongocxx::database &db) {

}