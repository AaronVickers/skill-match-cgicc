// CGI page header
#include "Utils/CGIPage.hpp"

// CGICC headers
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTTPCookie.h"

// Required headers
#include <ostream>

// Use required namespaces
using namespace std;
using namespace cgicc;

// Log  CGI page class structure
class LogOutCGIPage: public CGIPage {
private:
    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On POST request method
void LogOutCGIPage::onPOST(ostream &os) const {
    // Delete session token
    HTTPCookie deleteSessionCookie = HTTPCookie("SESSION_TOKEN", "DELETED; HttpOnly");
    deleteSessionCookie.setRemoved(true);

    // Redirect to login page with delete session cookie
    os << HTTPRedirectHeader("./login.cgi", false)
        .setCookie(deleteSessionCookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << LogOutCGIPage();

    return 0;
}
