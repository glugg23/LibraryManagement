#include <iostream>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "PRIVATE.h"
#include "user.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

int main() {
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{DATABASE_LOGIN}};
    mongocxx::database db = client["library_test"];

    User user;

    do {
        std::string username, password;
        std::cout << "Enter your username: ";
        std::cin >> username;
        std::cout << "Enter your password: ";
        std::cin >> password;

        //Set values of user
        user(username, password);

        //Find if document matching username exists
        core::optional<bsoncxx::document::value> maybeDoc
            = db[USERS].find_one(make_document(kvp("username", user.getUsername())));

        if(maybeDoc) {
            //Get element in password row
            auto element = maybeDoc->view()["password"];

            if(user.getPassword() == element.get_utf8().value.to_string()) {
                user.toggleLoggedIn();
                std::cout << "You have successfully logged in!" << std::endl;

            } else {
                std::cout << "Wrong username or password." << std::endl;
            }

        } else {
            std::cout << "This user does not exist." << std::endl;
            char input;
            do {
                std::cout << "Would you like to create a new user? (y/n): ";
                std::cin >> input;

                if(input == 'y' || input == 'Y') {
                    auto result = db[USERS].insert_one(make_document(
                        kvp("username", user.getUsername()),
                        kvp("password", user.getPassword()),
                        kvp("borrowedBooks", make_array())
                    ));

                    user.toggleLoggedIn();
                    std::cout << "You have successfully logged in!" << std::endl;
                    break;
                }

            } while(!(input == 'n' || input == 'N'));
        }

    } while(!user.isLoggedIn());

    return 0;
}