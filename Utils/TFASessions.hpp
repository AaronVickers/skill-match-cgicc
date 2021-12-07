// Header guard
#ifndef TFA_SESSIONS_H_
#define TFA_SESSIONS_H_

// Define environment constants
#define MAX_TFA_ATTEMPTS 5
#define MAX_TFA_SESSION_LENGTH 300

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include <ctime>
#include "Utils/Users.hpp"

// 2FA class structure
class TFASession {
private:
    int tfaSessionId;
    int userId;
    std::string token;
    std::string code;
    time_t startTime;
    int failedAttempts;
    bool authenticated;
public:
    int getTFASessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    std::string getCode();
    time_t getStartTime();
    int getFailedAttempts();
    bool getAuthenticated();

    void increaseFailedAttempts();
    void setAuthenticated(bool _authenticated);

    bool submitCode(std::string _code);
    bool isExpired();

    TFASession(int _tfaSessionId);
    TFASession(User user);
    TFASession(int _tfaSessionId, int _userId, std::string _token, std::string _code, time_t _startTime, int _failedAttempts, bool _authenticated);
};

// 2FA result class structure
class TFAResult: public Result {
public:
    TFASession *tfaSession;
};

// End of header guard
#endif
