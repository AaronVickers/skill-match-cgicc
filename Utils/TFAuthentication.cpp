// Include header file
#include "Utils/TFAuthentication.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/sole.hpp"

int TFAuthentication::getTFAuthenticationId() {
    // Return 2FA ID
    return TFAuthenticationId;
}

int TFAuthentication::getUserId() {
    // Return user ID
    return userId;
}

User TFAuthentication::getUser() {
    // Get 2FA user
    User user = User(userId);

    // Return user
    return user;
}

std::string TFAuthentication::getToken() {
    // Return token
    return token;
}

std::string TFAuthentication::getCode() {
    // Return code
    return code;
}

sql::Timestamp TFAuthentication::getStartTime() {
    // Return start time
    return startTime;
}

int TFAuthentication::getFailedAttempts() {
    // Return failed attempts
    return failedAttempts;
}

bool TFAuthentication::getAuthenticated() {
    // Return authenticated
    return authenticated;
}

bool TFAuthentication::submitCode(std::string _code) {
    // Create code validity status
    bool wasCodeValid;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Check if code is valid
    if (code.compare(_code) == 0) {
        // Set code validity status
        wasCodeValid = true;

        // SQL statement variable
        sql::PreparedStatement *pstmt;

        // Prepare 2FA update statement
        pstmt = db.conn->prepareStatement(" \
            UPDATE TFAuthentication \
            SET Authenticated=? \
            WHERE TFAuthenticationId=? \
        ");

        // Execute query
        pstmt->setBoolean(1, wasCodeValid);
        pstmt->setInt(2, TFAuthenticationId);
        pstmt->execute();

        // Delete statement from memory
        delete pstmt;

        // Update authenticated
        authenticated = wasCodeValid;
    } else {
        // Set code validity status
        wasCodeValid = false;

        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare 2FA update statement
        pstmt = db.conn->prepareStatement(" \
            UPDATE TFAuthentication \
            SET FailedAttempts=FailedAttempts+1 \
            WHERE TFAuthenticationId=? \
        ");

        // Execute query
        pstmt->setInt(1, TFAuthenticationId);
        pstmt->execute();

        // Prepare 2FA select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM TFAuthentication WHERE TFAuthenticationId=?");

        // Execute query
        pstmt->setInt(1, TFAuthenticationId);
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

    // Return whether code was valid
    return wasCodeValid;
}

TFAuthentication::TFAuthentication(int _TFAuthenticationId) {
    // Set 2FA ID
    TFAuthenticationId = _TFAuthenticationId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare 2FA select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM TFAuthentication WHERE TFAuthenticationId=?");

    // Execute query
    pstmt->setInt(1, TFAuthenticationId);
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

TFAuthentication::TFAuthentication(User user) {
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
    pstmt = db.conn->prepareStatement("INSERT INTO TFAuthentication (UserId, Token, Code, StartTime, FailedAttempts, Authenticated) VALUES (?,?,?,?,?,?)");

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
    TFAuthenticationId = res->getInt(1);

    // Delete result from memory
    delete res;
}

TFAuthentication::TFAuthentication(int _TFAuthenticationId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime, int _failedAttempts, bool _authenticated) {
    // Initialise all attributes with passed parameters
    TFAuthenticationId = _TFAuthenticationId;
    userId = _userId;
    token = _token;
    code = _code;
    startTime = _startTime;
    failedAttempts = _failedAttempts;
    authenticated = _authenticated;
}
