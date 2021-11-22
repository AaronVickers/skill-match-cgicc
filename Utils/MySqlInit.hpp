// Header guard
#ifndef MYSQL_INIT_H_
#define MYSQL_INIT_H_

// Define environment constants
#define DB_HOST "tcp://127.0.0.1:3306"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "coursework"

// MySQL headers
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>

// Required headers
#include "Utils/Result.hpp"

// MySqlInit class structure
class MySqlInit: public Result {
private:
    // SQL driver
    sql::Driver *driver;
public:
    // SQL connection to database
    sql::Connection *conn;

    // Constructor
    MySqlInit();

    // Destructor
    ~MySqlInit();
};

// End of header guard
#endif
