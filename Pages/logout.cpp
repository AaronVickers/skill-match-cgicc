// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised CGICC header
#include "Utils/CGICCInit.hpp"

// CGICC headers
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTTPCookie.h"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Sessions.hpp"
#include "Utils/Users.hpp"

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
    // Whether to log out from all sessions
    cgicc::form_iterator fromAllSessionsFormEntry = CGICCInit::cgi->getElement("fromAllSessions");
    bool fromAllSessions = !fromAllSessionsFormEntry->isEmpty();

    // Get list of cookies
    std::vector<cgicc::HTTPCookie> cookies = CGICCInit::env->getCookieList();

    // Placeholder for session cookie and found status
    bool sessionCookieFound = false;
    cgicc::HTTPCookie sessionCookie;

    // Find session cookie
    for (auto &cookie: cookies) {
        if (cookie.getName().compare("SESSION_TOKEN") == 0) {
            sessionCookieFound = true;
            sessionCookie = cookie;

            break;
        }
    }

    // Check if session cookie exists
    if (sessionCookieFound) {
        // Attempt to get session from token
        SessionResult sessionResult = Authentication::getSessionByToken(sessionCookie.getValue());
        if (sessionResult.getSuccess()) {
            // Handle log out type
            if (fromAllSessions) {
                // Get user of current session
                User user = sessionResult.session->getUser();

                // Deactivate all of user's sessions
                Sessions::deactivateAllSessions(user);
            } else {
                // Set active state of current session to false
                sessionResult.session->setActive(false);
            }
        }
    }

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
