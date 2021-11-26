// Header guard
#ifndef TFAUTHENTICATION_H_
#define TFAUTHENTICATION_H_

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Users.hpp"

// 2FA class structure
class TFAuthentication {
private:
    int TFAuthenticationId;
    int userId;
    std::string token;
    std::string code;
    sql::Timestamp startTime;
    int failedAttempts;
    bool authenticated;
public:
    int getTFAuthenticationId();
    int getUserId();
    User getUser();
    std::string getToken();
    std::string getCode();
    sql::Timestamp getStartTime();
    int getFailedAttempts();
    bool getAuthenticated();

    bool submitCode(std::string _code);

    TFAuthentication(int _TFAuthenticationId);
    TFAuthentication(User user);
    TFAuthentication(int _TFAuthenticationId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime, int _failedAttempts, bool _authenticated);
};

// 2FA result class structure
class TFAResult: public Result {
public:
    TFAuthentication *tfaSession;
};

#endif
