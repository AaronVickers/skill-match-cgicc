// Header guard
#ifndef USER_H_
#define USER_H_

// Required headers
#include <string>
#include "Utils/Roles.hpp"

// Forward declaration of required classes
class Role;

// User class structure
class User {
private:
    int userId;
    std::string username;
    std::string email;
    std::string passwordHash;
    std::string passwordSalt;
    int roleId;
public:
    int getUserId();
    std::string getUsername();
    std::string getEmail();
    int getRoleId();
    Role getRole();
};

// End of header guard
#endif