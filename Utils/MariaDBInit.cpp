// Include header file
#include "Utils/MariaDBInit.hpp"

// MariaDB headers
#include <mariadb/conncpp.hpp>

// Constructor
MariaDBInit::MariaDBInit() {
    try {
        // Connect to database
        driver = sql::mariadb::get_driver_instance();
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
MariaDBInit::~MariaDBInit() {
    delete conn;
}
