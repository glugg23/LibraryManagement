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
        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(doc["borrowedBy"]["isBorrowed"].get_bool()) {
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
        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(doc["borrowedBy"]["isBorrowed"].get_bool()) {
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
        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(doc["borrowedBy"]["isBorrowed"].get_bool()) {
            std::cout << ", currently unavailable" << std::endl;

        } else {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}

void searchBookAdvanced(User &user, mongocxx::database &db) {
    //Flush buffer as we're now using getline rather than just cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string input, title, author, genre;

    std::cout << "Search by title? (y/n): ";
    std::getline(std::cin, input);

    if(input == "y") {
        std::cout << "Enter title: ";
        std::getline(std::cin, title);
    }

    std::cout << "Search by author? (y/n): ";
    std::getline(std::cin, input);

    if(input == "y") {
        std::cout << "Enter author: ";
        std::getline(std::cin, author);
    }

    std::cout << "Search by genre? (y/n): ";
    std::getline(std::cin, input);

    if(input == "y") {
        std::cout << "Enter genre: ";
        std::getline(std::cin, genre);
    }

    auto cursor = db[BOOKS].find(
        make_document(
            kvp("title", make_document(kvp("$regex", title), kvp("$options", "i"))),
            kvp("author", make_document(kvp("$regex", author), kvp("$options", "i"))),
            kvp("genre", make_document(kvp("$regex", genre), kvp("$options", "i")))
        )
    );

    std::cout << std::endl;

    //Shows error but still works
    for(const bsoncxx::document::view &doc : cursor) {
        std::cout << doc["title"].get_utf8().value
                  << " by " << doc["author"].get_utf8().value
                  << ", in genre " << doc["genre"].get_utf8().value;

        if(doc["borrowedBy"]["isBorrowed"].get_bool()) {
            std::cout << ", currently unavailable" << std::endl;

        } else {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}