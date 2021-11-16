#ifndef MYSQL_INIT_H_
#define MYSQL_INIT_H_

// Define environment variables
#define DB_ADDR "tcp://127.0.0.1:3306"
#define DB_USER "DB_USERNAME"
#define DB_PASS "DB_PASSWORD"

// MySQL includes
#include "mysql_connection.h"
#include "mysql_driver.h"

// MySQL C++ Connector includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace mysqlInit {
    sql::Driver *sqlDriver = get_driver_instance();
    sql::Connection *sqlConn = sqlDriver->connect(DB_ADDR, DB_USER, DB_PASS);
}

#endif
