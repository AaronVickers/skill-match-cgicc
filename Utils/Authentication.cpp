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

// Required headers
#include <string>
#include <regex>
#include "Utils/Users.hpp"
#include "Utils/Roles.hpp"

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

    // Verify that all fields have a value
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

    // Check username format
    std::regex usernameRegex = std::regex("^[a-zA-Z0-9]{3,20}$");
    std::smatch usernameMatch;
    bool usernameValid = std::regex_match(username, usernameMatch, usernameRegex);

    // Handle invalid username format
    if (!usernameValid) {
        registerResult.setError("invalid_username");

        return registerResult;
    }

    // Check email format
    std::regex emailRegex = std::regex("(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])");
    std::smatch emailMatch;
    bool emailValid = std::regex_match(email, emailMatch, emailRegex);

    // Handle invalid email format
    if (!emailValid) {
        registerResult.setError("invalid_email");

        return registerResult;
    }

    // Check password format
    std::regex passwordRegex = std::regex(".{8,}");
    std::smatch passwordMatch;
    bool passwordValid = std::regex_match(password, passwordMatch, passwordRegex);

    // Handle invalid password format
    if (!passwordValid) {
        registerResult.setError("invalid_password");

        return registerResult;
    }

    // Check skill format
    std::regex skillRegex = std::regex(".{3,}");
    std::smatch skillMatch;
    bool skillValid = std::regex_match(skill, skillMatch, skillRegex);

    // Handle invalid skill format
    if (!skillValid) {
        registerResult.setError("invalid_skill");

        return registerResult;
    }

    // Validate role existence
    RoleResult roleResult = Roles::getRoleByName(role);

    // Admin role validation
    if (role.compare("Administrator") == 0) {
        if (Users::doesAdminExist()) {
            roleResult.setError("An administrator already exists.");
        }
    }

    // Handle invalid role
    if (!roleResult.getSuccess()) {
        registerResult.setError("invalid_role");

        return registerResult;
    }

    // TODO: Generate random salt
    std::string salt = "TemporarySalt";

    // Variables for password hashing
    const char *passwordCString = password.c_str();
    const char *saltCString = salt.c_str();
    char hashCString[HASH_LEN];
    char encodedCString[ENCODED_LEN];

    // Hash password
    argon2_hash(
        T_COST, M_COST, PARALLELISM,
        passwordCString, password.length(),
        saltCString, salt.length(),
        hashCString, HASH_LEN,
        encodedCString, ENCODED_LEN,
        Argon2_id, ARGON2_VERSION_13
    );

    // Convert password hash to C++ string
    std::string passwordHashEncoded = encodedCString;

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
        pstmt->setInt(4, roleResult.role->getRoleId());

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
