// Header guard
#ifndef TOTP_SESSIONS_H_
#define TOTP_SESSIONS_H_

// Define environment constants
#define MAX_TOTP_ATTEMPTS 5
#define MAX_TOTP_SESSION_LENGTH 300

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include <ctime>
#include "Utils/Users.hpp"

// TOTP class structure
class TOTPSession {
private:
    int totpSessionId;
    int userId;
    std::string token;
    time_t startTime;
    int failedAttempts;
    bool authenticated;
public:
    int getTOTPSessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    time_t getStartTime();
    int getFailedAttempts();
    bool getAuthenticated();

    void increaseFailedAttempts();
    void setAuthenticated(bool _authenticated);

    bool submitCode(std::string _code);
    bool isExpired();

    TOTPSession(int _totpSessionId);
    TOTPSession(User user);
    TOTPSession(int _totpSessionId, int _userId, std::string _token, time_t _startTime, int _failedAttempts, bool _authenticated);
};

// TOTP result class structure
class TOTPResult: public Result {
public:
    TOTPSession *totpSession;
};

// End of header guard
#endif
