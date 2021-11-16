// Header guard
#ifndef CGICC_INIT_H_
#define CGICC_INIT_H_

// CGICC headers
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"

// CgiccInit class structure
class CgiccInit {
private:
    cgicc::Cgicc cgi;
public:
    const cgicc::CgiEnvironment &env = cgi.getEnvironment();

    // Constructor
    // CgiccInit();
};

// End of header guard
#endif