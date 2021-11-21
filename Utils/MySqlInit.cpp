// Include header file
#include "Utils/MySqlInit.hpp"

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

        // Set success
        setSuccess(true);
    } catch (sql::SQLException &sql_error) {
        // Set error
        setError(sql_error.what());
    }
}

// Destructor
MySqlInit::~MySqlInit() {
    delete conn;
}
