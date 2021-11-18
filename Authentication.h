// Header guard
#ifndef AUTHENTICATION_INIT_H_
#define AUTHENTICATION_INIT_H_

// Required headers
#include <string>
#include "Result.h"

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

// Authentication class structure
class Authentication {
public:
    LoginResult Login(std::string username, std::string password);

    TFAResult SubmitTFA(std::string token, std::string code);

    Result Register(std::string username, std::string email, std::string password, std::string skill, std::string role);
};

// End of header guard
#endif
