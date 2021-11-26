// Header guard
#ifndef USER_H_
#define USER_H_

// Required headers
#include <string>
#include <vector>
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
    bool locked;
public:
    int getUserId();
    std::string getUsername();
    std::string getEmail();
    std::string getPasswordHashEncoded();
    int getRoleId();
    Role getRole();
    bool getLocked();

    void setLocked(bool _locked);

    User(int _userId);
    User(std::string _username, std::string _email, std::string password, Role &role);
    User(int _userId, std::string _username, std::string _email, std::string _passwordHashEncoded, int _roleId, bool _locked);
};

// User result class structure
class UserResult: public Result {
public:
    User *user;
};

// Users result class structure
class UsersResult: public Result {
public:
    std::vector<User> users;
};

// Users namespace
namespace Users {
    UserResult getUserByUsername(std::string username);

    bool doesAdminExist();
}

// End of header guard
#endif
