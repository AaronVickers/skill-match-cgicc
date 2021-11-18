// Header guard
#ifndef USER_H_
#define USER_H_

// Required headers
#include <string>

// User class structure
class User {
private:
    std::string username;
    std::string email;
    std::string passwordHash;
    std::string passwordSalt;
    std::string skill;
    std::string role;
public:
    std::string getUsername();
    std::string getEmail();
    std::string getSkill();
    std::string getRole();
};

// End of header guard
#endif
