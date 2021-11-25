// Header guard
#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

// Define environment constants
#define T_COST 2
#define M_COST 15360
#define PARALLELISM 1
#define HASH_LEN 32
#define ENCODED_LEN 128

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Result.hpp"
#include "Utils/Users.hpp"

// Forward declaration of required classes
class User;

// Register result class structure
class RegisterResult: public Result {};

// TFA class structure
class TFAuthentication {
private:
    int TFAuthenticationId;
    int userId;
    std::string token;
    std::string code;
    sql::Timestamp startTime;
public:
    int getTFAuthenticationId();
    int getUserId();
    User getUser();
    std::string getToken();
    std::string getCode();
    sql::Timestamp getStartTime();

    TFAuthentication(int _TFAuthenticationId);
    TFAuthentication(User user);
    TFAuthentication(int _TFAuthenticationId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime);
};

// Login result class structure
class LoginResult: public Result {
public:
    TFAuthentication *tfaSession;
};

// Session class structure
class Session {
private:
    int sessionId;
    int userId;
    std::string token;
    sql::Timestamp startTime;
public:
    int getSessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    sql::Timestamp getStartTime();

    Session(int _TFAuthenticationId);
    Session(User user);
    Session(int _TFAuthenticationId, int _userId, std::string _token, sql::Timestamp _startTime);
};

// 2FA result class structure
class TFAResult: public Result {
public:
    Session *session;
};

// Authentication namespace
namespace Authentication {
    LoginResult login(std::string username, std::string password);

    TFAResult submitTFA(std::string TFAToken, std::string code);

    RegisterResult registerAccount(std::string username, std::string email, std::string password, std::string skill, std::string role);

    UserResult getUserFromSessionToken(std::string sessionToken);

    std::string generateTFACode(int codeLength);
}

// End of header guard
#endif
