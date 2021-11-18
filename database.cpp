// Initialised header
#include "MySqlInit.h"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

// Required headers
#include <vector>

// Use required namespaces
using namespace std;

// MySqlInit constructor
MySqlInit::MySqlInit() {
    try {
        driver = get_driver_instance();
        conn = driver->connect(DB_HOST, DB_USER, DB_PASS);

        success = true;
    } catch (sql::SQLException &sql_error) {
        success = false;
        errorMsg = sql_error.what();
    }
}

// Entry function
int main(int argc, char *argv[]) {
    // Vector of roles
    vector<string> roles;
    roles.emplace_back("Administrator");
    roles.emplace_back("Company");
    roles.emplace_back("Applicant");

    // Get confirmation of operation
    string confirm;
    cout << "Are you sure you want to drop and repopulate the " DB_NAME " database? [y/N] ";
    cin >> confirm;

    // Validate confirmation
    if (confirm.compare("y") != 0 && confirm.compare("Y") != 0) {
        cout << "Operation cancelled." << endl;

        return 0;
    }

    // Initialise MySQL connection
    MySqlInit *db = new MySqlInit();

    // Handle connection error
    if (db->success == false) {
        cout << "Failed to connect to database.\nError: " << db->errorMsg << endl;

        return 0;
    }

    // Attempt to complete operation
    try {
        // SQL statement variables
        sql::Statement *stmt = db->conn->createStatement();
        sql::PreparedStatement *pstmt;

        // Drop and create database
        stmt->execute("DROP DATABASE IF EXISTS " DB_NAME ";");
        stmt->execute("CREATE DATABASE " DB_NAME ";");

        // Set schema to new database
        db->conn->setSchema(DB_NAME);

        // Create 'Roles' table
        stmt->execute(" \
            CREATE TABLE Roles ( \
                RoleId INT NOT NULL AUTO_INCREMENT, \
                Name VARCHAR(128) NOT NULL, \
                PRIMARY KEY (RoleId) \
            ); \
        ");

        // Create 'Users' table
        stmt->execute(" \
            CREATE TABLE Users ( \
                UserId INT NOT NULL AUTO_INCREMENT, \
                Username VARCHAR(20) NOT NULL, \
                Email VARCHAR(320) NOT NULL, \
                PasswordHash VARCHAR(128) NOT NULL, \
                PasswordSalt VARCHAR(128) NOT NULL, \
                RoleId INT NOT NULL, \
                PRIMARY KEY (UserId), \
                FOREIGN KEY (RoleId) REFERENCES Roles(RoleId) \
            ); \
        ");

        // Create 'TFAuthentication' table
        stmt->execute(" \
            CREATE TABLE TFAuthentication ( \
                TFAuthenticationId INT NOT NULL AUTO_INCREMENT, \
                UserId INT NOT NULL, \
                Token VARCHAR(128) NOT NULL, \
                Code VARCHAR(6) NOT NULL, \
                StartTime DATETIME NOT NULL, \
                PRIMARY KEY (TFAuthenticationId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Create 'Sessions' table
        stmt->execute(" \
            CREATE TABLE Sessions ( \
                SessionId INT NOT NULL AUTO_INCREMENT, \
                UserId INT NOT NULL, \
                Token VARCHAR(128) NOT NULL, \
                StartTime DATETIME NOT NULL, \
                PRIMARY KEY (SessionId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Create 'Skills' table
        stmt->execute(" \
            CREATE TABLE Skills ( \
                SkillId INT NOT NULL AUTO_INCREMENT, \
                Name VARCHAR(128) NOT NULL, \
                PRIMARY KEY (SkillId) \
            ); \
        ");

        // Create 'SkillSearches' table
        stmt->execute(" \
            CREATE TABLE SkillSearches ( \
                SkillSearchId INT NOT NULL AUTO_INCREMENT, \
                SkillId INT NOT NULL, \
                UserId INT NOT NULL, \
                ApprovedState BOOLEAN NOT NULL, \
                PRIMARY KEY (SkillSearchId), \
                FOREIGN KEY (SkillId) REFERENCES Skills(SkillId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Prepare role insertion statement
        pstmt = db->conn->prepareStatement("INSERT INTO Roles (Name) VALUES (?)");

        // Populate 'Roles' table
        for (auto role: roles) {
            pstmt->setString(1, role);
            pstmt->execute();
        }

        // Notify user that operation was completed
        cout << "Operation complete." << endl;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        cout << "Failed to create database.\nError: " << sql_error.what() << endl;
    }

    return 0;
}
