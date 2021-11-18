// Header guard
#ifndef CGICC_INIT_H_
#define CGICC_INIT_H_

// CGICC headers
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"

// Required headers
#include "Result.h"

// CgiccInit class structure
class CgiccInit: public Result {
private:
    // Cgicc instance
    cgicc::Cgicc cgi;
public:
    // Cgicc environment
    const cgicc::CgiEnvironment &env = cgi.getEnvironment();

    // Constructor
    CgiccInit();
};

// End of header guard
#endif
