// Header guard
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

// Required headers
#include <string>
#include "Utils/Result.hpp"
#include "Utils/Users.hpp"

// Forward declaration of required classes
class User;

// Login result class structure
class LoginResult: public Result {
public:
    std::string TFAToken;
};

// 2FA result class structure
class TFAResult: public Result {
public:
    std::string sessionToken;
};

// Register result class structure
class RegisterResult: public Result {};

// User result class structure
class UserResult: public Result {
public:
    User *user;
};

// Authentication class structure
class Authentication {
public:
    LoginResult login(std::string username, std::string password);

    TFAResult submitTFA(std::string TFAToken, std::string code);

    RegisterResult registerAccount(std::string username, std::string email, std::string password, std::string skill, std::string role);

    UserResult getUserFromSessionToken(std::string sessionToken);
};

// End of header guard
#endif
