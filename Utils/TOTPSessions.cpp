// Include header file
#include "Utils/TOTPSessions.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include <ctime>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/sole.hpp"

int TOTPSession::getTOTPSessionId() {
    // Return TOTP ID
    return totpSessionId;
}

int TOTPSession::getUserId() {
    // Return user ID
    return userId;
}

User TOTPSession::getUser() {
    // Get TOTP user
    User user = User(userId);

    // Return user
    return user;
}

std::string TOTPSession::getToken() {
    // Return token
    return token;
}

time_t TOTPSession::getStartTime() {
    // Return start time
    return startTime;
}

int TOTPSession::getFailedAttempts() {
    // Return failed attempts
    return failedAttempts;
}

bool TOTPSession::getAuthenticated() {
    // Return authenticated
    return authenticated;
}

void TOTPSession::increaseFailedAttempts() {
    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare TOTP update statement
    pstmt = db.conn->prepareStatement(" \
        UPDATE TOTPSessions \
        SET FailedAttempts=FailedAttempts+1 \
        WHERE TOTPSessionId=? \
    ");

    // Execute query
    pstmt->setInt(1, totpSessionId);
    pstmt->execute();

    // Prepare TOTP select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM TOTPSessions WHERE TOTPSessionId=?");

    // Execute query
    pstmt->setInt(1, totpSessionId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get TOTP failed attempts from row
    failedAttempts = res->getInt("FailedAttempts");

    // Delete result from memory
    delete res;
}

void TOTPSession::setAuthenticated(bool newAuthenticated) {
    // Avoid updating with no change
    if (authenticated == newAuthenticated) {
        return;
    }

    // Update authenticated
    authenticated = newAuthenticated;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;

    // Prepare TOTP update statement
    pstmt = db.conn->prepareStatement(" \
        UPDATE TOTPSessions \
        SET Authenticated=? \
        WHERE TOTPSessionId=? \
    ");

    // Execute query
    pstmt->setBoolean(1, authenticated);
    pstmt->setInt(2, totpSessionId);
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;
}

bool TOTPSession::submitCode(std::string _code) {
    // Check if exceeded failed attempts count
    if (failedAttempts >= MAX_TOTP_ATTEMPTS) {
        // Return code invalid
        return false;
    }

    // Get current time
    time_t time1 = std::time(0);
    // Round time down to multiple of 30
    time_t timeSeed1 = time1 - time1 % 30;

    // Generate TOTP code for current time
    std::string totpCode1 = Authentication::generateTOTPCode(timeSeed1, 6);

    // Check for valid code
    if (totpCode1.compare(_code) == 0) {
        // Set authenticated state
        setAuthenticated(true);

        // Return code valid
        return true;
    }

    // Calculate current time - 30
    time_t time2 = time1 - 30;
    // Round time down to multiple of 30
    time_t timeSeed2 = time2 - time2 % 30;

    // Generate TOTP code for current time - 30
    std::string totpCode2 = Authentication::generateTOTPCode(timeSeed2, 6);

    // Check for valid code
    if (totpCode2.compare(_code) == 0) {
        // Set authenticated state
        setAuthenticated(true);

        // Return code valid
        return true;
    }

    // Calculate current time + 30
    time_t time3 = time1 + 30;
    // Round time down to multiple of 30
    time_t timeSeed3 = time3 - time3 % 30;

    // Generate TOTP code for current time + 30
    std::string totpCode3 = Authentication::generateTOTPCode(timeSeed3, 6);

    // Check for valid code
    if (totpCode3.compare(_code) == 0) {
        // Set authenticated state
        setAuthenticated(true);

        // Return code valid
        return true;
    }

    // Increase failed attempts
    increaseFailedAttempts();

    // Return code invalid
    return false;
}

bool TOTPSession::isExpired() {
    // Calculate session length
    double totpSessionLength = std::difftime(std::time(0), startTime);

    // Check if session length is over expiry time
    if (totpSessionLength > MAX_TOTP_SESSION_LENGTH) {
        // Return true for expired
        return true;
    }

    // Return false for not expired
    return false;
}

TOTPSession::TOTPSession(int _totpSessionId) {
    // Set TOTP ID
    totpSessionId = _totpSessionId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare TOTP select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM TOTPSessions WHERE TOTPSessionId=?");

    // Execute query
    pstmt->setInt(1, totpSessionId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get TOTP details from row
    userId = res->getInt("UserId");
    token = res->getString("Token").c_str();
    startTime = res->getInt("StartTime");
    failedAttempts = res->getInt("FailedAttempts");
    authenticated = res->getBoolean("Authenticated");

    // Delete result from memory
    delete res;
}

TOTPSession::TOTPSession(User user) {
    // Set user ID
    userId = user.getUserId();

    // Generate and set token
    sole::uuid tokenUUID = sole::uuid4();
    token = tokenUUID.str();

    // Get and set start time
    startTime = std::time(0);

    // Set failed attempts
    failedAttempts = 0;

    // Set authenticated
    authenticated = false;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variables
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare TOTP insert
    pstmt = db.conn->prepareStatement("INSERT INTO TOTPSessions (UserId, Token, StartTime, FailedAttempts, Authenticated) VALUES (?,?,?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, userId);
    pstmt->setString(2, token);
    pstmt->setInt(3, startTime);
    pstmt->setInt(4, failedAttempts);
    pstmt->setBoolean(5, authenticated);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get TOTP ID from row
    totpSessionId = res->getInt(1);

    // Delete result from memory
    delete res;
}

TOTPSession::TOTPSession(int _totpSessionId, int _userId, std::string _token, time_t _startTime, int _failedAttempts, bool _authenticated) {
    // Initialise all attributes with passed parameters
    totpSessionId = _totpSessionId;
    userId = _userId;
    token = _token;
    startTime = _startTime;
    failedAttempts = _failedAttempts;
    authenticated = _authenticated;
}
