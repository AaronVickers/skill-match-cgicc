// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Component headers
#include "Components/TFAForm.hpp"
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/Roles.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// 2FA CGI page class structure
class TFACGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void TFACGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect(TFA_PAGE);
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Two-Factor Authentication")) << endl;
    os << body();

    // Page heading
    os << h1("Two-Factor Authentication");

    // Display 2FA form
    os << TFAForm();

    // Cancel link
    os << cgicc::a("Cancel")
        .set("href", "./login.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void TFACGIPage::onPOST(ostream &os) const {
    // Get list of cookies
    vector<HTTPCookie> cookies = env.getCookieList();

    // Placeholder for 2FA token cookie and found status
    bool tfaTokenCookieFound = false;
    HTTPCookie tfaTokenCookie;

    // Find 2FA token cookie
    for (auto &cookie: cookies) {
        if (cookie.getName().compare("TFA_TOKEN") == 0) {
            tfaTokenCookieFound = true;
            tfaTokenCookie = cookie;

            break;
        }
    }

    // Check if 2FA token cookie was found
    if (!tfaTokenCookieFound) {
        // Redirect to login page with error message
        os << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // Get token from cookie
    string token = tfaTokenCookie.getValue();

    // Get form data
    string code = cgi.getElement("code")->getValue();

    // Submit 2FA token and code
    TFASubmitResult tfaResult = Authentication::submitTFA(token, code);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!tfaResult.getSuccess()) {
        string errorMsg = tfaResult.getErrorMsg();

        if (errorMsg.compare("invalid_tfa_session") == 0 || errorMsg.compare("account_locked") == 0) {
            // Redirect to login page with error message
            redirectLocation = "./login.cgi?error=" + errorMsg;
        } else {
            // Redirect to 2FA page with error message
            redirectLocation = "./tfa.cgi?error=" + errorMsg;
        }

        os << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // TODO: Admin hardware token implementation

    // Get user of session
    User user = tfaResult.session->getUser();

    // Get role of user
    Role role = user.getRole();
    std::string roleName = role.getName();

    // Get corresponding page for user
    if (roleName.compare("Administrator") == 0) {
        redirectLocation = "./admin.cgi";
    } else if (roleName.compare("Company") == 0) {
        redirectLocation = "./company.cgi";
    } else if (roleName.compare("Applicant") == 0) {
        redirectLocation = "./applicant.cgi";
    }

    // Create session cookie
    string sessionCookieValue = tfaResult.session->getToken() + "; HttpOnly";
    HTTPCookie sessionCookie = HTTPCookie("SESSION_TOKEN", sessionCookieValue);

    // Delete 2FA token
    HTTPCookie deleteTFACookie = HTTPCookie("TFA_TOKEN", "DELETED; HttpOnly");
    deleteTFACookie.setRemoved(true);

    // Redirect to corresponding page with session token cookie
    os << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(sessionCookie)
        .setCookie(deleteTFACookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << TFACGIPage();

    return 0;
}
