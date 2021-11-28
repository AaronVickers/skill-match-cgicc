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

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

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

    // Attempt to get 2FA session from token
    TFAResult tfaResult = Authentication::getTFAByToken(tfaTokenCookie.getValue());
    if (!tfaResult.getSuccess()) {
        // Redirect to login page with error message
        os << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // Handle already authenticated 2FA session
    if (tfaResult.tfaSession->getAuthenticated()) {
        // Redirect to login page with error message
        os << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // TODO: Handle expired 2FA session

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Two-Factor Authentication")) << endl;
    os << body();

    // Display 2FA form
    os << TFAForm();

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

    // Create session cookie
    HTTPCookie sessionTokenCookie = HTTPCookie("SESSION_TOKEN", tfaResult.session->getToken());

    // TODO: Get corresponding page for user
    redirectLocation = "./applicant.cgi";

    // Redirect to corresponding page with session token cookie
    os << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(sessionTokenCookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << TFACGIPage();

    return 0;
}
