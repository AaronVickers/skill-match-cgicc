// Include header file
#include "Utils/Authentication.hpp"

// Initialised MySQL header
#include "Utils/MySqlInit.hpp"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

// Argon2 header
#include "argon2.h"

LoginResult Authentication::login(std::string username, std::string password) {
    // Create result
    LoginResult loginResult = LoginResult();

    // TODO: Validate data format

    // TODO: Validate credentials

    // TODO: Generate 2FA session

    // Set result success
    loginResult.setSuccess(true);
    
    // Return result
    return loginResult;
}

TFAResult Authentication::submitTFA(std::string token, std::string code) {
    // Create result
    TFAResult tfaResult = TFAResult();

    // TODO: Validate data format

    // TODO: Validate 2FA code

    // TODO: Generate authenticated session

    // Set result success
    tfaResult.setSuccess(true);
    
    // Return result
    return tfaResult;
}

RegisterResult Authentication::registerAccount(std::string username, std::string email, std::string password, std::string skill, std::string role) {
    // Create result
    RegisterResult registerResult = RegisterResult();

    if (username.empty()) {
        registerResult.setError("missing_username");

        return registerResult;
    } else if (email.empty()) {
        registerResult.setError("missing_email");

        return registerResult;
    } else if (password.empty()) {
        registerResult.setError("missing_password");

        return registerResult;
    } else if (skill.empty()) {
        registerResult.setError("missing_skill");

        return registerResult;
    } else if (role.empty()) {
        registerResult.setError("missing_role");

        return registerResult;
    }

    // TODO: Validate data format

    // TODO: Generate registered details
    std::string passwordHashEncoded = "temp";
    int roleId = 2;

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

    // Handle connection error
    if (!db.getSuccess()) {
        // Set success to false and store error
        registerResult.setError(db.getErrorMsg());

        // Return result
        return registerResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variables
        sql::Statement *stmt = db.conn->createStatement();
        sql::PreparedStatement *pstmt;

        // Prepare user insertion statement
        pstmt = db.conn->prepareStatement("INSERT INTO Users (Username, Email, PasswordHashEncoded, RoleId) VALUES (?,?,?,?)");

        // Insert values into statement
        pstmt->setString(1, username);
        pstmt->setString(2, email);
        pstmt->setString(3, passwordHashEncoded);
        pstmt->setInt(4, roleId);

        // Execute statement
        pstmt->execute();
    } catch (sql::SQLException &sql_error) {
        // Set success to false and store error
        registerResult.setError(sql_error.what());

        // Return result
        return registerResult;
    }
    
    // Set result success
    registerResult.setSuccess(true);

    // Return result
    return registerResult;
}

/*
UserResult Authentication::getUserFromSessionToken(std::string sessionToken) {
    // Create result
    UserResult userResult = UserResult();

    // TODO: Validate data format

    // TODO: Find user by session token
    
    // Set result success
    userResult.setSuccess(true);
    
    // Return result
    return userResult;
}
*/
