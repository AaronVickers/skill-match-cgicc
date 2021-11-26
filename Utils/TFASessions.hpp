// Header guard
#ifndef TFA_SESSIONS_H_
#define TFA_SESSIONS_H_

// Define environment constants
#define MAX_TFA_ATTEMPTS 5

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Users.hpp"

// 2FA class structure
class TFASession {
private:
    int tfaSessionId;
    int userId;
    std::string token;
    std::string code;
    sql::Timestamp startTime;
    int failedAttempts;
    bool authenticated;
public:
    int getTFASessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    std::string getCode();
    sql::Timestamp getStartTime();
    int getFailedAttempts();
    bool getAuthenticated();

    void increaseFailedAttempts();
    void setAuthenticated(bool _authenticated);

    bool submitCode(std::string _code);

    TFASession(int _tfaSessionId);
    TFASession(User user);
    TFASession(int _tfaSessionId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime, int _failedAttempts, bool _authenticated);
};

// 2FA result class structure
class TFAResult: public Result {
public:
    TFASession *tfaSession;
};

#endif
