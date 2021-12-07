// Header guard
#ifndef SESSIONS_H_
#define SESSIONS_H_

// Define environment constants
#define MAX_SESSION_LENGTH 86400

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include <ctime>
#include "Utils/Users.hpp"

// Session class structure
class Session {
private:
    int sessionId;
    int userId;
    std::string token;
    time_t startTime;
    bool active;
public:
    int getSessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    time_t getStartTime();
    bool getActive();

    void setActive(bool _active);
    bool isExpired();

    Session(int _sessionId);
    Session(User user);
    Session(int _sessionId, int _userId, std::string _token, time_t _startTime, bool _active);
};

// Session result class structure
class SessionResult: public Result {
public:
    Session *session;
};

// End of header guard
#endif
