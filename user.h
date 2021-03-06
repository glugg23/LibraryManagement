#pragma once

#include <iostream>

class User {
private:
    std::string username;
    std::string password;
    bool loggedIn = false;

public:
    User() = default;

    User(const std::string &username, const std::string &password);

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    std::string getPassword() const;

    void setPassword(const std::string &password);

    bool isLoggedIn() const;

    void toggleLoggedIn();

    void operator()(const std::string &username, const std::string &password);
};