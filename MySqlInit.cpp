// Include header file
#include "MySqlInit.h"

// MySQL headers
#include <cppconn/exception.h>

// Constructor
MySqlInit::MySqlInit() {
    try {
        // Connect to database
        driver = get_driver_instance();
        conn = driver->connect(DB_HOST, DB_USER, DB_PASS);

        // Set schema to use
        conn->setSchema(DB_NAME);

        // Set success to true
        success = true;
    } catch (sql::SQLException &sql_error) {
        // Set success to false and store error
        success = false;
        errorMsg = sql_error.what();
    }
}
