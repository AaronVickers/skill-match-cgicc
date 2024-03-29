// Header guard
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

// Define environment constants
#define T_COST 2
#define M_COST 15360
#define PARALLELISM 1
#define HASH_LEN 32
#define ENCODED_LEN 128
#define PEPPER "RANDOM_PEPPER_VALUE"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Result.hpp"
#include "Utils/Users.hpp"
#include "Utils/TFASessions.hpp"
#include "Utils/TOTPSessions.hpp"
#include "Utils/Sessions.hpp"

// Forward declaration of required classes
class User;

// Register result class structure
class RegisterResult: public UserResult {};

// Login result class structure
class LoginResult: public TFAResult {};

// 2FA result class structure
class TFASubmitResult: public Result {
public:
    bool totpRequired;
    TOTPSession *totpSession;
    Session *session;
};

// TOTP result class structure
class TOTPSubmitResult: public SessionResult {};

// Authentication namespace
namespace Authentication {
    RegisterResult registerAccount(std::string username, std::string email, std::string password, std::string skill, std::string role);

    LoginResult login(std::string username, std::string password);

    TFASubmitResult submitTFA(std::string token, std::string code);

    TOTPSubmitResult submitTOTP(std::string token, std::string code);

    TFAResult getTFAByToken(std::string token);

    TOTPResult getTOTPByToken(std::string token);

    SessionResult getSessionByToken(std::string token);

    std::string generateTFACode(int codeLength);

    std::string generateTOTPCode(int seed, int codeLength);
}

// End of header guard
#endif
