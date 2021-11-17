// Include header file
#include "MySqlInit.h"

// MySQL headers
#include <cppconn/exception.h>

// Constructor
MySqlInit::MySqlInit() {
    try {
        driver = get_driver_instance();
        conn = driver->connect(DB_ADDR, DB_USER, DB_PASS);

        conn->setSchema("coursework");

        success = true;
    } catch (sql::SQLException &sql_error) {
        success = false;
        error_msg = sql_error.what();
    }
}
