// Header guard
#ifndef CGICC_INIT_H_
#define CGICC_INIT_H_

// CGICC headers
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"

// CGICCInit namespace structure
namespace CGICCInit {
    // Cgicc instance
    extern cgicc::Cgicc *cgi;

    // Cgicc environment
    extern const cgicc::CgiEnvironment *env;
}

// End of header guard
#endif
