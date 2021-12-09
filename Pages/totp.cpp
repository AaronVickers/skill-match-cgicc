// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised CGICC header
#include "Utils/CGICCInit.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Component headers
#include "Components/CodeForm.hpp"
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/Roles.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// TOTP CGI page class structure
class TOTPPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void TOTPPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect(TOTP_PAGE);
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Time-based One Time Password")) << endl;
    os << body();

    // Page heading
    os << h1("Time-based One Time Password");

    // Page description
    os << p("Enter your time-based one time password.");

    // Warning
    os << p("Warning: Your account will be locked after 5 failed attempts.");

    // Display code form
    os << CodeForm();

    // Cancel link
    os << cgicc::a("Cancel")
        .set("href", "./login.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void TOTPPage::onPOST(ostream &os) const {
    // Get list of cookies
    vector<HTTPCookie> cookies = CGICCInit::env->getCookieList();

    // Placeholder for TOTP cookie and found status
    bool totpCookieFound = false;
    HTTPCookie totpCookie;

    // Find TOTP cookie
    for (auto &cookie: cookies) {
        if (cookie.getName().compare("TOTP_TOKEN") == 0) {
            totpCookieFound = true;
            totpCookie = cookie;

            break;
        }
    }

    // Check if TOTP cookie was found
    if (!totpCookieFound) {
        // Redirect to login page with error message
        os << HTTPRedirectHeader("./login.cgi?error=invalid_totp_session", false) << endl;

        return;
    }

    // Get token from cookie
    string token = totpCookie.getValue();

    // Get form data
    string code = CGICCInit::cgi->getElement("code")->getValue();

    // Submit TOTP token and code
    TOTPSubmitResult totpResult = Authentication::submitTOTP(token, code);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!totpResult.getSuccess()) {
        string errorMsg = totpResult.getErrorMsg();

        if (errorMsg.compare("invalid_totp_session") == 0 || errorMsg.compare("account_locked") == 0) {
            // Redirect to login page with error message
            redirectLocation = "./login.cgi?error=" + errorMsg;
        } else {
            // Redirect to TOTP page with error message
            redirectLocation = "./totp.cgi?error=" + errorMsg;
        }

        os << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Get user of session
    User user = totpResult.session->getUser();

    // Get role of user
    Role role = user.getRole();
    std::string roleName = role.getName();

    // Get corresponding page for user
    if (roleName.compare(ADMINISTRATOR_ROLE_NAME) == 0) {
        redirectLocation = "./admin.cgi";
    } else if (roleName.compare(APPLICANT_ROLE_NAME) == 0) {
        redirectLocation = "./applicant.cgi";
    } else if (roleName.compare(COMPANY_ROLE_NAME) == 0) {
        redirectLocation = "./company.cgi";
    }

    // Create session cookie
    string sessionCookieValue = totpResult.session->getToken() + "; HttpOnly";
    HTTPCookie sessionCookie = HTTPCookie("SESSION_TOKEN", sessionCookieValue);

    // Delete TOTP token
    HTTPCookie deleteTOTPCookie = HTTPCookie("TOTP_TOKEN", "DELETED; HttpOnly");
    deleteTOTPCookie.setRemoved(true);

    // Redirect to corresponding page with session cookie
    os << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(sessionCookie)
        .setCookie(deleteTOTPCookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << TOTPPage();

    return 0;
}
