// Include header file
#include "Utils/Session.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Users.hpp"
#include "Utils/sole.hpp"

int Session::getSessionId() {
    // Return session ID
    return sessionId;
}

int Session::getUserId() {
    // Return user ID
    return userId;
}

User Session::getUser() {
    // Get session user
    User user = User(userId);

    // Return user
    return user;
}

std::string Session::getToken() {
    // Return token
    return token;
}

sql::Timestamp Session::getStartTime() {
    // Return start time
    return startTime;
}

bool Session::getActive() {
    // Return active
    return active;
}

void Session::setActive(bool _active) {
    // TODO: Set session active status
    active = _active;
}

Session::Session(int _sessionId) {
    // Set session ID
    sessionId = _sessionId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare session select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Sessions WHERE SessionId=?");

    // Execute query
    pstmt->setInt(1, sessionId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get session details from row
    userId = res->getInt("UserId");
    token = res->getString("Token").c_str();
    startTime = res->getString("StartTime");
    active = res->getBoolean("Active");

    // Delete result from memory
    delete res;
}

Session::Session(User user) {
    // Set user ID
    userId = user.getUserId();

    // Generate and set token
    sole::uuid tokenUUID = sole::uuid4();
    token = tokenUUID.str();

    // TODO: Get and set start time
    startTime = "TEMP";

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Set active
    active = true;

    // SQL statement variables
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare session insert
    pstmt = db.conn->prepareStatement("INSERT INTO Sessions (UserId, Token, StartTime, Active) VALUES (?,?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, userId);
    pstmt->setString(2, token);
    pstmt->setString(3, startTime);
    pstmt->setBoolean(4, active);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get session ID from row
    sessionId = res->getInt(1);

    // Delete result from memory
    delete res;
}

Session::Session(int _sessionId, int _userId, std::string _token, sql::Timestamp _startTime, bool _active) {
    // Initialise all attributes with passed parameters
    sessionId = _sessionId;
    userId = _userId;
    token = _token;
    startTime = _startTime;
    active = _active;
}
