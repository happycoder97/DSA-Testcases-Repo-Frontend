#include <string>
#include <optional>


struct User {
    std::string username;
    std::string password;
    bool is_admin;
};


std::optional<User> login();
