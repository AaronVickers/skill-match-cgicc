// Initialised headers
#include "CgiccInit.h"
#include "MySqlInit.h"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Entry function
int main(int argc, char *argv[]) {
    // Initialise CGICC environment
    CgiccInit *cgi = new CgiccInit();

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

    return 0;
}
