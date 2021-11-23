// Initialised headers
#include "Utils/CgiccInit.hpp"
#include "Utils/MySqlInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"

// Required headers
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

void onGET(CgiccInit &cgi) {
    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Login")) << endl;
    cout << body();

    // Form for login
    auto loginForm = form();
    loginForm.set("method", "post"); // Submit as post request

    // Username input
    loginForm.add(cgicc::div()
        .add(span("Username: "))
        .add(input()
            .set("type", "text")
            .set("name", "username")
            .set("placeholder", "Username")
        )
    );

    // Password input
    loginForm.add(cgicc::div()
        .add(span("Password: "))
        .add(input()
            .set("type", "password")
            .set("name", "password")
            .set("placeholder", "Password")
        )
    );

    // Login button
    loginForm.add(input()
        .set("type", "submit")
        .set("value", "Login")
    );

    // Register link
    loginForm.add(a("Register")
        .set("href", "./register.cgi")
    );

    // Display login form
    cout << loginForm;

    // End of response
    cout << body() << html();
}

void onPOST(CgiccInit &cgi) {
    // Get form data
    string username = cgi.cgi.getElement("username")->getValue();
    string password = cgi.cgi.getElement("password")->getValue();

    // Attempt login
    LoginResult loginResult = Authentication::login(username, password);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!loginResult.getSuccess()) {
        // Redirect to login page with error message
        redirectLocation = "./login.cgi?error=" + loginResult.getErrorMsg();
        cout << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Redirect to 2FA page
    redirectLocation = "./tfa.cgi";
    cout << HTTPRedirectHeader(redirectLocation, false) << endl;
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
