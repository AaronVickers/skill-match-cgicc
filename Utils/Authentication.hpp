// Header guard
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

// Required headers
#include <string>
#include "Utils/Result.hpp"
#include "Utils/User.hpp"

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

// User result class structure
class UserResult: public Result {
public:
    User user;
};

// Authentication class structure
class Authentication {
public:
    LoginResult Login(std::string username, std::string password);

    TFAResult SubmitTFA(std::string TFAToken, std::string code);

    Result Register(std::string username, std::string email, std::string password, std::string skill, std::string role);

    UserResult GetUserFromSessionToken(std::string sessionToken);
};

// End of header guard
#endif
