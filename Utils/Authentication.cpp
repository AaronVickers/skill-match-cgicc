// Include header file
#include "Utils/Authentication.hpp"

// Initialised MySQL header
#include "Utils/MySqlInit.hpp"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

LoginResult Authentication::Login(std::string username, std::string password) {
    // Create result
    LoginResult loginResult = LoginResult();

    // TODO: Validate data format

    // TODO: Validate credentials

    // TODO: Generate 2FA session

    // Set success to true
    loginResult.success = true;
    
    // Return result
    return loginResult;
}

TFAResult Authentication::SubmitTFA(std::string token, std::string code) {
    // Create result
    TFAResult tfaResult = TFAResult();

    // TODO: Validate data format

    // TODO: Validate 2FA code

    // TODO: Generate authenticated session

    // Set success to true
    tfaResult.success = true;
    
    // Return result
    return tfaResult;
}

Result Authentication::Register(std::string username, std::string email, std::string password, std::string skill, std::string role) {
    // Create result
    Result registerResult = Result();

    // TODO: Validate data format

    // TODO: Generate registered details

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

    // Handle connection error
    if (db.success == false) {
        // Set success to false and store error
        registerResult.success = false;
        registerResult.errorMsg = db.errorMsg;

        // Return result
        return registerResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variables
        sql::Statement *stmt = db.conn->createStatement();
        sql::PreparedStatement *pstmt;

        // Prepare user insertion statement
        pstmt = db.conn->prepareStatement("INSERT INTO Users (Username, Email, PasswordHash, PasswordSalt, Role) VALUES (?,?,?,?,?)");

        // pstmt->setString(1, username);
        // pstmt->setString(2, email);
        // pstmt->setString(3, passwordHash);
        // pstmt->setString(4, passwordSalt);
        // pstmt->setString(5, role);

        // pstmt->execute();

        // Set success to true
        registerResult.success = true;
    } catch (sql::SQLException &sql_error) {
        // Set success to false and store error
        registerResult.success = false;
        registerResult.errorMsg = sql_error.what();
    }

    // Return result
    return registerResult;
}

UserResult Authentication::GetUserFromSessionToken(std::string sessionToken) {
    // Create result
    UserResult userResult = UserResult();

    // TODO: Validate data format

    // TODO: Find user by session token

    // Set success to true
    userResult.success = true;
    
    // Return result
    return userResult;
}
