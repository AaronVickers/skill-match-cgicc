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
#include "Components/RegisterForm.hpp"
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Register CGI page class structure
class RegisterCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void RegisterCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect(UNAUTHENTICATED_PAGE);
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Register")) << endl;
    os << body();

    // Page heading
    os << h1("Register");

    // Display register form
    os << RegisterForm();

    // Login link
    os << cgicc::a("Login")
        .set("href", "./login.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void RegisterCGIPage::onPOST(ostream &os) const {
    // Get form data
    string username = cgi.getElement("username")->getValue();
    string email = cgi.getElement("email")->getValue();
    string password = cgi.getElement("password")->getValue();
    string skill = cgi.getElement("skill")->getValue();
    string role = cgi.getElement("role")->getValue();

    // Attempt to register
    RegisterResult registerResult = Authentication::registerAccount(username, email, password, skill, role);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!registerResult.getSuccess()) {
        // Redirect to register page with error message
        redirectLocation = "./register.cgi?error=" + registerResult.getErrorMsg();
        os << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Redirect to login page with message
    redirectLocation = "./login.cgi?message=registered";
    os << HTTPRedirectHeader(redirectLocation, false) << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << RegisterCGIPage();

    return 0;
}
