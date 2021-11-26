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
    // Get list of cookies
    vector<HTTPCookie> cookies = cgi.env.getCookieList();

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
        cout << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // Attempt to get 2FA session from token
    TFAResult tfaResult = Authentication::getTFAByToken(tfaTokenCookie.getValue());
    if (!tfaResult.getSuccess()) {
        // Redirect to login page with error message
        cout << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // Handle already authenticated 2FA session
    if (tfaResult.tfaSession->getAuthenticated()) {
        // Redirect to login page with error message
        cout << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // TODO: Handle expired 2FA session

    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Two-Factor Authentication")) << endl;
    cout << body();

    // Form for 2FA
    auto TFAForm = form();
    TFAForm.set("method", "post"); // Submit as post request

    // Code input
    TFAForm.add(cgicc::div()
        .add(span("Code: "))
        .add(input()
            .set("type", "text")
            .set("name", "code")
            .set("placeholder", "Code")
        )
    );

    // Submit button
    TFAForm.add(input()
        .set("type", "submit")
        .set("value", "Submit")
    );

    // Login link
    TFAForm.add(a("Login")
        .set("href", "./login.cgi")
    );

    // Display 2FA form
    cout << TFAForm;

    // End of response
    cout << body() << html();
}

void onPOST(CgiccInit &cgi) {
    // Get list of cookies
    vector<HTTPCookie> cookies = cgi.env.getCookieList();

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
        cout << HTTPRedirectHeader("./login.cgi?error=invalid_tfa_session", false) << endl;

        return;
    }

    // Get token from cookie
    string token = tfaTokenCookie.getValue();

    // Get form data
    string code = cgi.cgi.getElement("code")->getValue();

    // Submit 2FA token and code
    TFASubmitResult tfaResult = Authentication::submitTFA(token, code);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!tfaResult.getSuccess()) {
        string errorMsg = tfaResult.getErrorMsg();

        if (errorMsg.compare("invalid_tfa_session") == 0) {
            // Redirect to login page with error message
            redirectLocation = "./login.cgi?error=" + errorMsg;
        } else {
            // Redirect to 2FA page with error message
            redirectLocation = "./tfa.cgi?error=" + errorMsg;
        }

        cout << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Create session cookie
    HTTPCookie sessionTokenCookie = HTTPCookie("SESSION_TOKEN", tfaResult.session->getToken());

    // TODO: Get corresponding page for user
    redirectLocation = "./applicant.cgi";

    // Redirect to corresponding page with session token cookie
    cout << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(sessionTokenCookie)
        << endl;
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
