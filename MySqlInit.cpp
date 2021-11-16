// Include header file
#include "MySqlInit.h"

// Constructor
MySqlInit::MySqlInit() {
    driver = get_driver_instance();
    conn = driver->connect(DB_ADDR, DB_USER, DB_PASS);

    conn->setSchema("coursework");
}