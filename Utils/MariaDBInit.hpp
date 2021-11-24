// Header guard
#ifndef MARIADB_INIT_H_
#define MARIADB_INIT_H_

// Define environment constants
#define DB_HOST "tcp://127.0.0.1:3306"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "coursework"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include "Utils/Result.hpp"

// MariaDBInit class structure
class MariaDBInit: public Result {
private:
    // SQL driver
    sql::Driver *driver;
public:
    // SQL connection to database
    sql::Connection *conn;

    // Constructor
    MariaDBInit();

    // Destructor
    ~MariaDBInit();
};

// End of header guard
#endif
