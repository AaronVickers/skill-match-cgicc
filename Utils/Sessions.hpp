// Header guard
#ifndef SESSIONS_H_
#define SESSIONS_H_

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Users.hpp"

// Session class structure
class Session {
private:
    int sessionId;
    int userId;
    std::string token;
    sql::Timestamp startTime;
    bool active;
public:
    int getSessionId();
    int getUserId();
    User getUser();
    std::string getToken();
    sql::Timestamp getStartTime();
    bool getActive();

    void setActive(bool _active);

    Session(int _sessionId);
    Session(User user);
    Session(int _sessionId, int _userId, std::string _token, sql::Timestamp _startTime, bool _active);
};

// Session result class structure
class SessionResult: public Result {
public:
    Session *session;
};

#endif
