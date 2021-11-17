// Header guard
#ifndef MYSQL_INIT_H_
#define MYSQL_INIT_H_

// Define environment variables
#define DB_ADDR "tcp://127.0.0.1:3306"
#define DB_USER "DB_USERNAME"
#define DB_PASS "DB_PASSWORD"

// MySQL headers
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>

// MySqlInit class structure
class MySqlInit {
private:
    sql::Driver *driver;
public:
    bool success;
    std::string error_msg;

    sql::Connection *conn;

    // Constructor
    MySqlInit();
};

// End of header guard
#endif
