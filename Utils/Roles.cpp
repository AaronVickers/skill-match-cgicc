// Include header file
#include "Utils/Roles.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>

int Role::getRoleId() {
    // Return role ID
    return roleId;
}

std::string Role::getName() {
    // Return name
    return name;
}

Role::Role(int _roleId) {
    roleId = _roleId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare role select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Roles WHERE RoleId=?");

    // Execute query
    pstmt->setInt(1, roleId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get role name from row
    name = res->getString("Name").c_str();

    // Delete result from memory
    delete res;
}

Role::Role(int _roleId, std::string _name) {
    roleId = _roleId;
    name = _name;
}

RoleResult Roles::getRoleByName(std::string name) {
    // Create result
    RoleResult roleResult = RoleResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        roleResult.setError(db.getErrorMsg());

        // Return result
        return roleResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare role select statement
        pstmt = db.conn->prepareStatement("SELECT RoleId FROM Roles WHERE Name=?");

        // Execute query
        pstmt->setString(1, name);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            roleResult.setError("Role with provided name doesn't exist.");

            return roleResult;
        }

        // Get role ID from first row
        int roleId = res->getInt("RoleId");

        // Store role in result
        roleResult.role = new Role(roleId, name);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        roleResult.setError(sql_error.what());

        // Return result
        return roleResult;
    }

    // Set result success
    roleResult.setSuccess(true);

    // Return result
    return roleResult;
}
