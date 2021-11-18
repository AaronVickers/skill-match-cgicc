// Header guard
#ifndef MYSQL_INIT_H_
#define MYSQL_INIT_H_

// Define environment variables
#define DB_HOST "tcp://127.0.0.1:3306"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "coursework"

// MySQL headers
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>

// MySqlInit class structure
class MySqlInit {
private:
    // SQL driver
    sql::Driver *driver;
public:
    // Success status and error message for initialisation
    bool success;
    std::string error_msg;

    // SQL connection to database
    sql::Connection *conn;

    // Constructor
    MySqlInit();
};

// End of header guard
#endif
