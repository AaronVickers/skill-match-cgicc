// Include header file
#include "Utils/TFASessions.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/sole.hpp"

int TFASession::getTFASessionId() {
    // Return 2FA ID
    return tfaSessionId;
}

int TFASession::getUserId() {
    // Return user ID
    return userId;
}

User TFASession::getUser() {
    // Get 2FA user
    User user = User(userId);

    // Return user
    return user;
}

std::string TFASession::getToken() {
    // Return token
    return token;
}

std::string TFASession::getCode() {
    // Return code
    return code;
}

sql::Timestamp TFASession::getStartTime() {
    // Return start time
    return startTime;
}

int TFASession::getFailedAttempts() {
    // Return failed attempts
    return failedAttempts;
}

bool TFASession::getAuthenticated() {
    // Return authenticated
    return authenticated;
}

void TFASession::increaseFailedAttempts() {
    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare 2FA update statement
    pstmt = db.conn->prepareStatement(" \
        UPDATE TFASession \
        SET FailedAttempts=FailedAttempts+1 \
        WHERE TFASessionId=? \
    ");

    // Execute query
    pstmt->setInt(1, tfaSessionId);
    pstmt->execute();

    // Prepare 2FA select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM TFASession WHERE TFASessionId=?");

    // Execute query
    pstmt->setInt(1, tfaSessionId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get 2FA failed attempts from row
    failedAttempts = res->getInt("FailedAttempts");

    // Delete result from memory
    delete res;
}

void TFASession::setAuthenticated(bool newAuthenticated) {
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

    // Prepare 2FA update statement
    pstmt = db.conn->prepareStatement(" \
        UPDATE TFASession \
        SET Authenticated=? \
        WHERE TFASessionId=? \
    ");

    // Execute query
    pstmt->setBoolean(1, authenticated);
    pstmt->setInt(2, tfaSessionId);
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;
}

bool TFASession::submitCode(std::string _code) {
    // Check if exceeded failed attempts count
    if (failedAttempts >= MAX_TFA_ATTEMPTS) {
        // Return code invalid
        return false;
    }

    // Check for invalid code
    if (code.compare(_code) != 0) {
        // Increase failed attempts
        increaseFailedAttempts();

        // Return code invalid
        return false;
    }

    // Set authenticated state
    setAuthenticated(true);

    // Return code valid
    return true;
}

TFASession::TFASession(int _tfaSessionId) {
    // Set 2FA ID
    tfaSessionId = _tfaSessionId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare 2FA select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM TFASession WHERE TFASessionId=?");

    // Execute query
    pstmt->setInt(1, tfaSessionId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get 2FA details from row
    userId = res->getInt("UserId");
    token = res->getString("Token").c_str();
    code = res->getString("Code").c_str();
    startTime = res->getString("StartTime");
    failedAttempts = res->getInt("FailedAttempts");
    authenticated = res->getBoolean("Authenticated");

    // Delete result from memory
    delete res;
}

TFASession::TFASession(User user) {
    // Set user ID
    userId = user.getUserId();

    // Generate and set token
    sole::uuid tokenUUID = sole::uuid4();
    token = tokenUUID.str();

    // Generate and set code
    code = Authentication::generateTFACode(6);

    // TODO: Get and set start time
    startTime = "TEMP";

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

    // Prepare 2FA insert
    pstmt = db.conn->prepareStatement("INSERT INTO TFASession (UserId, Token, Code, StartTime, FailedAttempts, Authenticated) VALUES (?,?,?,?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, userId);
    pstmt->setString(2, token);
    pstmt->setString(3, code);
    pstmt->setString(4, startTime);
    pstmt->setInt(5, failedAttempts);
    pstmt->setBoolean(6, authenticated);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get 2FA ID from row
    tfaSessionId = res->getInt(1);

    // Delete result from memory
    delete res;
}

TFASession::TFASession(int _tfaSessionId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime, int _failedAttempts, bool _authenticated) {
    // Initialise all attributes with passed parameters
    tfaSessionId = _tfaSessionId;
    userId = _userId;
    token = _token;
    code = _code;
    startTime = _startTime;
    failedAttempts = _failedAttempts;
    authenticated = _authenticated;
}
