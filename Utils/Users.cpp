// Include header file
#include "Utils/Users.hpp"

// Initialised MySQL header
#include "Utils/MySqlInit.hpp"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

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

User::User(int _userId) {

}

User::User(std::string _username, std::string _email, std::string password, Role &role) {

}

User::User(int _userId, std::string _username, std::string _email, std::string _passwordHashEncoded, int _roleId) {
    userId = _userId;
    username = _username;
    email = _email;
    passwordHashEncoded = _passwordHashEncoded;
    roleId = _roleId;
}

UserResult Users::getUserByUsername(std::string username) {
    // Create result
    UserResult userResult = UserResult();

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

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

        // Prepare role select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM Users WHERE Username=?");

        // Execute query
        pstmt->setValue(1, username);
        res = pstmt->executeQuery();

        // Check if result has any rows
        bool foundRow = res->next();

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
        std::string username = res->getValue("Username");
        std::string email = res->getValue("Email");
        std::string passwordHashEncoded = res->getValue("PasswordHashEncoded");
        std::string roleId = res->getInt("RoleId");

        // Store role in result
        userResult.role = new User(userId, username, email, passwordHashEncoded, roleId);

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
    // Get admin role
    RoleResult adminRoleResult = Roles::getRoleByName("Administrator");

    // Fail-safe if there is an error getting admin role
    if (!adminRoleResult.getSuccess()) {
        return true;
    }

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

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

        // Prepare role select statement
        pstmt = db.conn->prepareStatement("SELECT 1 FROM Users WHERE RoleId=?");

        // Execute query
        pstmt->setInt(1, adminRoleResult.role->getRoleId());
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
