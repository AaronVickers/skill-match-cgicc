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
    // TODO: Redirect if not logged in as company
    // TODO: Show list of approved applicants with company's skill

    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Company")) << endl;
    cout << body();

    // Temporary response
    cout << h1("Company");
    cout << p("Temporary response");

    // End of response
    cout << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Initialise CGICC environment
    CgiccInit cgi = CgiccInit();

    // Handle different request types
    if (cgi.env.getRequestMethod() == "GET") {
        // Handle GET request
        onGET(cgi);
    }

    return 0;
}
