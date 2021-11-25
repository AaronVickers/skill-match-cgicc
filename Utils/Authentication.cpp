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

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare 2FA insert
    pstmt = db.conn->prepareStatement("INSERT INTO TFAuthentication (UserId, Token, Code, StartTime) VALUES (?,?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, userId);
    pstmt->setString(2, token);
    pstmt->setString(3, code);
    pstmt->setString(4, startTime);

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

TFAuthentication::TFAuthentication(int _TFAuthenticationId, int _userId, std::string _token, std::string _code, sql::Timestamp _startTime) {
    // Initialise all attributes with passed parameters
    TFAuthenticationId = _TFAuthenticationId;
    userId = _userId;
    token = _token;
    code = _code;
    startTime = _startTime;
}


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

    // SQL statement variable
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare session insert
    pstmt = db.conn->prepareStatement("INSERT INTO Sessions (UserId, Token, StartTime) VALUES (?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, userId);
    pstmt->setString(2, token);
    pstmt->setString(3, startTime);

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

Session::Session(int _sessionId, int _userId, std::string _token, sql::Timestamp _startTime) {
    // Initialise all attributes with passed parameters
    sessionId = _sessionId;
    userId = _userId;
    token = _token;
    startTime = _startTime;
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

    // Variables for password hashing
    const char *passwordCString = password.c_str();
    const char *encodedCString = passwordHashEncoded.c_str();

    // Verify password
    int verifySuccess = argon2_verify(
        encodedCString,
        passwordCString, password.length(),
        Argon2_id
    );

    // Handle incorrect password
    if (verifySuccess != ARGON2_OK) {
        loginResult.setError("incorrect_password");

        return loginResult;
    }

    // Generate 2FA session
    loginResult.tfaSession = new TFAuthentication(*userResult.user);

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

    // Check if username is in use
    UserResult existingUserResult = Users::getUserByUsername(username);
    if (existingUserResult.getSuccess()) {
        registerResult.setError("username_in_use");

        return registerResult;
    }

    // Generate unique salt
    sole::uuid saltUUID = sole::uuid4();
    std::string salt = saltUUID.str();

    // Variables for password hashing
    const char *passwordCString = password.c_str();
    const char *saltCString = salt.c_str();
    char hashCString[HASH_LEN];
    char encodedCString[ENCODED_LEN];

    // Hash password
    int hashSuccess = argon2_hash(
        T_COST, M_COST, PARALLELISM,
        passwordCString, password.length(),
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
        User newUser = User(username, email, passwordHashEncoded, *roleResult.role);
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
