#ifndef CGICC_INIT_H_
#define CGICC_INIT_H_

// CGICC includes
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

// Use CGICC namespace
using namespace cgicc;

namespace cgiccInit {
    Cgicc cgi;
    const CgiEnvironment &cgiEnv = cgi.getEnvironment();
}

#endif
