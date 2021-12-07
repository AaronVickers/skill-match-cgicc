// Include header file
#include "Utils/Authentication.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Argon2 header
#include "argon2.h"

// Required headers
#include <string>
#include <regex>
#include "Utils/Users.hpp"
#include "Utils/Roles.hpp"
#include "Utils/Skills.hpp"
#include "Utils/TFASessions.hpp"
#include "Utils/Sessions.hpp"
#include "Utils/sole.hpp"

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
    if (role.compare(ADMINISTRATOR_ROLE_NAME) == 0) {
        // Only allow 1 admin
        if (Users::doesAdminExist()) {
            roleResult.setError("An administrator already exists.");
        }
    }

    // Handle invalid role
    if (!roleResult.getSuccess()) {
        registerResult.setError("invalid_role");

        return registerResult;
    }

    // Check if username is in use
    UserResult existingUserResult = Users::getUserByUsername(username);
    if (existingUserResult.getSuccess()) {
        registerResult.setError("username_in_use");

        return registerResult;
    }

    // Generate unique salt
    sole::uuid saltUUID = sole::uuid4();
    std::string salt = saltUUID.str();

    // Add pepper to password
    std::string pepperedPassword = PEPPER + password;

    // Variables for password hashing
    const char *pepperedPasswordCString = pepperedPassword.c_str();
    const char *saltCString = salt.c_str();
    char hashCString[HASH_LEN];
    char encodedCString[ENCODED_LEN];

    // Hash password
    int hashSuccess = argon2_hash(
        T_COST, M_COST, PARALLELISM,
        pepperedPasswordCString, pepperedPassword.length(),
        saltCString, salt.length(),
        hashCString, HASH_LEN,
        encodedCString, ENCODED_LEN,
        Argon2_id, ARGON2_VERSION_13
    );

    // Handle failed hashing
    if (hashSuccess != ARGON2_OK) {
        registerResult.setError("invalid_password");

        return registerResult;
    }

    // Convert password hash to C++ string
    std::string passwordHashEncoded = encodedCString;

    // Attempt to complete operation
    try {
        // Get skill by name
        Skill userSkill = Skill(skill);

        // Create new user
        User newUser = User(username, email, passwordHashEncoded, *roleResult.role);

        // Create new skill search
        SkillSearch userSkillSearch(userSkill, newUser);
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

LoginResult Authentication::login(std::string username, std::string password) {
    // Create result
    LoginResult loginResult = LoginResult();

    // Verify that all fields have a value
    if (username.empty()) {
        loginResult.setError("missing_username");

        return loginResult;
    } else if (password.empty()) {
        loginResult.setError("missing_password");

        return loginResult;
    }

    // Make sure user exists
    UserResult userResult = Users::getUserByUsername(username);
    if (!userResult.getSuccess()) {
        loginResult.setError("invalid_username");

        return loginResult;
    }

    // Get password hash encoded
    std::string passwordHashEncoded = userResult.user->getPasswordHashEncoded();

    // Add pepper to password
    std::string pepperedPassword = PEPPER + password;

    // Variables for password hashing
    const char *pepperedPasswordCString = pepperedPassword.c_str();
    const char *encodedCString = passwordHashEncoded.c_str();

    // Verify password
    int verifySuccess = argon2_verify(
        encodedCString,
        pepperedPasswordCString, pepperedPassword.length(),
        Argon2_id
    );

    // Handle incorrect password
    if (verifySuccess != ARGON2_OK) {
        loginResult.setError("incorrect_password");

        return loginResult;
    }

    // Handle locked account
    if (userResult.user->getLocked()) {
        loginResult.setError("account_locked");

        return loginResult;
    }

    // Generate 2FA session
    loginResult.tfaSession = new TFASession(*userResult.user);

    // Set result success
    loginResult.setSuccess(true);

    // Return result
    return loginResult;
}

TFASubmitResult Authentication::submitTFA(std::string token, std::string code) {
    // Create result
    TFASubmitResult tfaSubmitResult = TFASubmitResult();

    // Verify that all fields have a value
    if (token.empty()) {
        tfaSubmitResult.setError("missing_token");

        return tfaSubmitResult;
    } else if (code.empty()) {
        tfaSubmitResult.setError("missing_code");

        return tfaSubmitResult;
    }

    // Attempt to get 2FA session from token
    TFAResult tfaResult = Authentication::getTFAByToken(token);
    if (!tfaResult.getSuccess()) {
        tfaSubmitResult.setError("invalid_tfa_session");

        return tfaSubmitResult;
    }

    // Handle already authenticated 2FA session
    if (tfaResult.tfaSession->getAuthenticated()) {
        tfaSubmitResult.setError("invalid_tfa_session");

        return tfaSubmitResult;
    }

    // Handle expired 2FA session
    if (tfaResult.tfaSession->isExpired()) {
        tfaSubmitResult.setError("expired_tfa_session");

        return tfaSubmitResult;
    }

    // Check code format
    std::regex codeRegex = std::regex("^[0-9]{6}$");
    std::smatch codeMatch;
    bool codeValid = std::regex_match(code, codeMatch, codeRegex);

    // Handle invalid code format
    if (!codeValid) {
        // Set error
        tfaSubmitResult.setError("invalid_code");

        // Return result
        return tfaSubmitResult;
    }

    // Submit code
    bool wasValidCode = tfaResult.tfaSession->submitCode(code);

    // Lock account after failed guesses
    if (tfaResult.tfaSession->getFailedAttempts() >= MAX_TFA_ATTEMPTS) {
        // Get user
        User tfaUser = tfaResult.tfaSession->getUser();

        // Update locked status
        tfaUser.setLocked(true);

        // Set error
        tfaSubmitResult.setError("account_locked");

        // Return result
        return tfaSubmitResult;
    }

    // Validate 2FA code
    if (!wasValidCode) {
        // Set error
        tfaSubmitResult.setError("invalid_code");

        // Return result
        return tfaSubmitResult;
    }

    // Generate authenticated session
    tfaSubmitResult.session = new Session(tfaResult.tfaSession->getUser());

    // Set result success
    tfaSubmitResult.setSuccess(true);

    // Return result
    return tfaSubmitResult;
}

TFAResult Authentication::getTFAByToken(std::string token) {
    // Create result
    TFAResult tfaResult = TFAResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        tfaResult.setError(db.getErrorMsg());

        // Return result
        return tfaResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare 2FA select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM TFASessions WHERE Token=?");

        // Execute query
        pstmt->setString(1, token);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            tfaResult.setError("2FA session with provided token doesn't exist.");

            return tfaResult;
        }

        // Get 2FA details from row
        int tfaSessionId = res->getInt("TFASessionId");
        int userId = res->getInt("UserId");
        std::string token = res->getString("Token").c_str();
        std::string code = res->getString("Code").c_str();
        time_t startTime = res->getInt("StartTime");
        int failedAttempts = res->getInt("FailedAttempts");
        bool authenticated = res->getBoolean("Authenticated");

        // Store TFA session in result
        tfaResult.tfaSession = new TFASession(tfaSessionId, userId, token, code, startTime, failedAttempts, authenticated);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        tfaResult.setError(sql_error.what());

        // Return result
        return tfaResult;
    }

    // Set result success
    tfaResult.setSuccess(true);

    // Return result
    return tfaResult;
}

SessionResult Authentication::getSessionByToken(std::string token) {
    // Create result
    SessionResult sessionResult = SessionResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        sessionResult.setError(db.getErrorMsg());

        // Return result
        return sessionResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare session select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM Sessions WHERE Token=?");

        // Execute query
        pstmt->setString(1, token);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            sessionResult.setError("Session with provided token doesn't exist.");

            return sessionResult;
        }

        // Get session details from row
        int sessionId = res->getInt("SessionId");
        int userId = res->getInt("UserId");
        std::string token = res->getString("Token").c_str();
        time_t startTime = res->getInt("StartTime");
        bool active = res->getBoolean("Active");

        // Store session in result
        sessionResult.session = new Session(sessionId, userId, token, startTime, active);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        sessionResult.setError(sql_error.what());

        // Return result
        return sessionResult;
    }

    // Set result success
    sessionResult.setSuccess(true);

    // Return result
    return sessionResult;
}

std::string Authentication::generateTFACode(int codeLength) {
    // Use Mersenne Twister engine with random device seed
    std::random_device rd;
    std::mt19937 generator(rd());

    // Define distribution of characters
    std::uniform_int_distribution<int> distribution('0', '9');

    // 2FA code string filled with null bytes
    std::string tfaCode(codeLength, '\0');

    // For each character in 2FA code string
    for(auto &dis: tfaCode) {
        // Replace with random character
        dis = distribution(generator);
    }

    // Return 2FA code
    return tfaCode;
}
