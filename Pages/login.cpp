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
#include "Components/LoginForm.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Login CGI page class structure
class LoginCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void LoginCGIPage::onGET(ostream &os) const {
    // TODO: Redirect if logged in

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Login")) << endl;
    os << body();

    // Page heading
    os << h1("Login");

    // Display login form
    os << LoginForm();

    // End of response
    os << body() << html();
}

// On POST request method
void LoginCGIPage::onPOST(ostream &os) const {
    // Get form data
    string username = cgi.getElement("username")->getValue();
    string password = cgi.getElement("password")->getValue();

    // Attempt login
    LoginResult loginResult = Authentication::login(username, password);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!loginResult.getSuccess()) {
        // Redirect to login page with error message
        redirectLocation = "./login.cgi?error=" + loginResult.getErrorMsg();
        os << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // TODO: Send email with 2FA code

    // Create 2FA token cookie
    HTTPCookie tfaTokenCookie = HTTPCookie("TFA_TOKEN", loginResult.tfaSession->getToken());

    // Redirect to 2FA page with 2FA token cookie
    redirectLocation = "./tfa.cgi";
    os << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(tfaTokenCookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << LoginCGIPage();

    return 0;
}
