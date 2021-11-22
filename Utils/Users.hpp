// Header guard
#ifndef USER_H_
#define USER_H_

// Required headers
#include <string>
#include "Utils/Result.hpp"
#include "Utils/Roles.hpp"

// Forward declaration of required classes
class Role;

// User class structure
class User {
private:
    int userId;
    std::string username;
    std::string email;
    std::string passwordHashEncoded;
    int roleId;
public:
    int getUserId();
    std::string getUsername();
    std::string getEmail();
    std::string getPasswordHashEncoded();
    int getRoleId();
    Role getRole();

    User(int _userId);
    User(std::string _username, std::string _email, std::string password, Role &role);
    User(int _userId, std::string _username, std::string _email, std::string password, int _roleId);
};

// User result class structure
class UserResult: public Result {
public:
    User user;
};

// Users namespace
namespace Users {
    UserResult getUserByUsername(std::string username);

    bool doesAdminExist();
}

// End of header guard
#endif
