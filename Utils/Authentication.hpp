// Header guard
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

// Define environment constants
#define T_COST 2
#define M_COST 15360
#define PARALLELISM 1
#define HASH_LEN 32
#define ENCODED_LEN 128

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

// Authentication namespace
namespace Authentication {
    LoginResult login(std::string username, std::string password);

    TFAResult submitTFA(std::string TFAToken, std::string code);

    RegisterResult registerAccount(std::string username, std::string email, std::string password, std::string skill, std::string role);

    UserResult getUserFromSessionToken(std::string sessionToken);
}

// End of header guard
#endif
