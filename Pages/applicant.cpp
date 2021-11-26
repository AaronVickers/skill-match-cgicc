// Initialised headers
#include "Utils/CgiccInit.hpp"
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Required headers
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

void onGET(CgiccInit &cgi) {
    
}

void onPOST(CgiccInit &cgi) {
    
}

// Entry function
int main(int argc, char *argv[]) {
    // Initialise CGICC environment
    CgiccInit cgi = CgiccInit();

    // Handle different request types
    if (cgi.env.getRequestMethod() == "GET") {
        // Handle GET request
        onGET(cgi);
    } else if (cgi.env.getRequestMethod() == "POST") {
        // Handle POST request
        onPOST(cgi);
    }

    return 0;
}
