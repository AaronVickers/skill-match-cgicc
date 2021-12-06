// Include header file
#include "Utils/CGICCInit.hpp"

// CGICC headers
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"

// CGICCInit namespace structure
namespace CGICCInit {
    // Cgicc instance
    cgicc::Cgicc *cgi = new cgicc::Cgicc();

    // Cgicc environment
    const cgicc::CgiEnvironment *env = &cgi->getEnvironment();
}
