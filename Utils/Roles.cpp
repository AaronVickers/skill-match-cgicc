// Include header file
#include "Utils/Roles.hpp"

// Initialised MySQL header
#include "Utils/MySqlInit.hpp"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

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
    
}

Role::Role(int _roleId, std::string _name) {
    roleId = _roleId;
    name = _name;
}

RoleResult Roles::getRoleByName(std::string name) {
    // Create result
    RoleResult roleResult = RoleResult();

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

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
