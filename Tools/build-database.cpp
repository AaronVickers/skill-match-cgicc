// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <iostream>
#include <vector>

// Use required namespaces
using namespace std;

// MariaDBInit constructor
MariaDBInit::MariaDBInit() {
    try {
        // Connect to database
        driver = sql::mariadb::get_driver_instance();
        conn = driver->connect(DB_HOST, DB_USER, DB_PASS);

        // Set success
        setSuccess(true);
    } catch (sql::SQLException &sql_error) {
        // Set error
        setError(sql_error.what());
    }
}

// MariaDBInit destructor
MariaDBInit::~MariaDBInit() {
    delete conn;
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
    cout << "Are you sure you want to drop and rebuild the " DB_NAME " database? [y/N] ";
    cin >> confirm;

    // Validate confirmation
    if (confirm.compare("y") != 0 && confirm.compare("Y") != 0) {
        cout << "Operation cancelled." << endl;

        return 0;
    }

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        cout << "Failed to connect to database.\nError: " << db.getErrorMsg() << endl;

        return 0;
    }

    // Attempt to complete operation
    try {
        // SQL statement variables
        sql::Statement *stmt = db.conn->createStatement();
        sql::PreparedStatement *pstmt;

        // Drop and create database
        stmt->execute("DROP DATABASE IF EXISTS " DB_NAME ";");
        stmt->execute("CREATE DATABASE " DB_NAME ";");

        // Set schema to new database
        db.conn->setSchema(DB_NAME);

        // Create 'Roles' table
        stmt->execute(" \
            CREATE TABLE Roles ( \
                RoleId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                Name VARCHAR(128) NOT NULL UNIQUE, \
                PRIMARY KEY (RoleId) \
            ); \
        ");

        // Create 'Users' table
        stmt->execute(" \
            CREATE TABLE Users ( \
                UserId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                Username VARCHAR(20) NOT NULL UNIQUE, \
                Email VARCHAR(320) NOT NULL, \
                PasswordHashEncoded VARCHAR(128) NOT NULL, \
                RoleId INT UNSIGNED NOT NULL, \
                PRIMARY KEY (UserId), \
                FOREIGN KEY (RoleId) REFERENCES Roles(RoleId) \
            ); \
        ");

        // Create 'TFAuthentication' table
        stmt->execute(" \
            CREATE TABLE TFAuthentication ( \
                TFAuthenticationId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                UserId INT UNSIGNED NOT NULL, \
                Token VARCHAR(128) NOT NULL, \
                Code VARCHAR(6) NOT NULL, \
                StartTime VARCHAR(128) NOT NULL, \
                PRIMARY KEY (TFAuthenticationId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Create 'Sessions' table
        stmt->execute(" \
            CREATE TABLE Sessions ( \
                SessionId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                UserId INT UNSIGNED NOT NULL, \
                Token VARCHAR(128) NOT NULL, \
                StartTime VARCHAR(128) NOT NULL, \
                PRIMARY KEY (SessionId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Create 'Skills' table
        stmt->execute(" \
            CREATE TABLE Skills ( \
                SkillId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                Name VARCHAR(128) NOT NULL UNIQUE, \
                PRIMARY KEY (SkillId) \
            ); \
        ");

        // Create 'SkillSearches' table
        stmt->execute(" \
            CREATE TABLE SkillSearches ( \
                SkillSearchId INT UNSIGNED NOT NULL UNIQUE AUTO_INCREMENT, \
                SkillId INT UNSIGNED NOT NULL, \
                UserId INT UNSIGNED NOT NULL, \
                ApprovedState BOOLEAN NOT NULL, \
                PRIMARY KEY (SkillSearchId), \
                FOREIGN KEY (SkillId) REFERENCES Skills(SkillId), \
                FOREIGN KEY (UserId) REFERENCES Users(UserId) \
            ); \
        ");

        // Prepare role insertion statement
        pstmt = db.conn->prepareStatement("INSERT INTO Roles (Name) VALUES (?)");

        // Populate 'Roles' table
        for (auto role: roles) {
            pstmt->setString(1, role);
            pstmt->execute();
        }

        // Delete statements from memory
        delete stmt;
        delete pstmt;

        // Notify user that operation was completed
        cout << "Operation complete." << endl;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        cout << "Failed to create database.\nError: " << sql_error.what() << endl;
    }

    return 0;
}
