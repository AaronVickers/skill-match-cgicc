// Include header file
#include "Utils/Users.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>
#include "Utils/Roles.hpp"

int User::getUserId() {
    // Return user ID
    return userId;
}

std::string User::getUsername() {
    // Return username
    return username;
}

std::string User::getEmail() {
    // Return email
    return email;
}

std::string User::getPasswordHashEncoded() {
    // Return password hash encoded
    return passwordHashEncoded;
}

int User::getRoleId() {
    // Return role
    return roleId;
}

Role User::getRole() {
    // Get user's role
    Role role = Role(roleId);

    // Return role
    return role;
}

bool User::getLocked() {
    // Return role
    return locked;
}

void User::setLocked(bool newLocked) {
    // Avoid updating with no change
    if (locked == newLocked) {
        return;
    }

    // Set account locked status
    locked = newLocked;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;

    // Prepare user update statement
    pstmt = db.conn->prepareStatement(" \
        UPDATE Users \
        SET Locked=? \
        WHERE UserId=? \
    ");

    // Execute query
    pstmt->setBoolean(1, locked);
    pstmt->setInt(2, userId);
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;
}

User::User(int _userId) {
    // Set user ID
    userId = _userId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare user select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Users WHERE UserId=?");

    // Execute query
    pstmt->setInt(1, userId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get user details from row
    username = res->getString("Username").c_str();
    email = res->getString("Email").c_str();
    passwordHashEncoded = res->getString("PasswordHashEncoded").c_str();
    roleId = res->getInt("RoleId");
    locked = res->getBoolean("Locked");

    // Delete result from memory
    delete res;
}

User::User(std::string _username, std::string _email, std::string _passwordHashEncoded, Role &role) {
    // Initialise attributes with passed parameters
    username = _username;
    email = _email;
    passwordHashEncoded = _passwordHashEncoded;
    roleId = role.getRoleId();
    locked = false;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variables
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare user insert
    pstmt = db.conn->prepareStatement("INSERT INTO Users (Username, Email, PasswordHashEncoded, RoleId, Locked) VALUES (?,?,?,?,?)");

    // Insert values into statement
    pstmt->setString(1, username);
    pstmt->setString(2, email);
    pstmt->setString(3, passwordHashEncoded);
    pstmt->setInt(4, roleId);
    pstmt->setBoolean(5, locked);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get user ID from row
    userId = res->getInt(1);

    // Delete result from memory
    delete res;
}

User::User(int _userId, std::string _username, std::string _email, std::string _passwordHashEncoded, int _roleId, bool _locked) {
    // Initialise all attributes with passed parameters
    userId = _userId;
    username = _username;
    email = _email;
    passwordHashEncoded = _passwordHashEncoded;
    roleId = _roleId;
    locked = _locked;
}

UserResult Users::getUserByUsername(std::string username) {
    // Create result
    UserResult userResult = UserResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        userResult.setError(db.getErrorMsg());

        // Return result
        return userResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare user select statement ignoring username case
        pstmt = db.conn->prepareStatement("SELECT * FROM Users WHERE Username=?");

        // Execute query
        pstmt->setString(1, username);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            userResult.setError("User with provided username doesn't exist.");

            return userResult;
        }

        // Get user details from first row
        int userId = res->getInt("UserId");
        std::string username = res->getString("Username").c_str();
        std::string email = res->getString("Email").c_str();
        std::string passwordHashEncoded = res->getString("PasswordHashEncoded").c_str();
        int roleId = res->getInt("RoleId");
        bool locked = res->getBoolean("Locked");

        // Store user in result
        userResult.user = new User(userId, username, email, passwordHashEncoded, roleId, locked);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        userResult.setError(sql_error.what());

        // Return result
        return userResult;
    }

    // Set result success
    userResult.setSuccess(true);

    // Return result
    return userResult;
}

bool Users::doesAdminExist() {
    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        return true;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare user select statement
        pstmt = db.conn->prepareStatement(" \
            SELECT * FROM Users \
            INNER JOIN Roles ON Roles.RoleId=Users.RoleId \
            WHERE Roles.Name=? \
        ");

        // Execute query
        pstmt->setString(1, ADMINISTRATOR_ROLE_NAME);
        res = pstmt->executeQuery();

        // Check if result has any rows
        bool foundRow = res->next();

        // Delete statement and result from memory
        delete pstmt;
        delete res;

        // If a row is found, an admin exists
        if (foundRow) {
            return true;
        }
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        return true;
    }

    return false;
}
