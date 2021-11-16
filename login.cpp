// Include initialised headers
#include "cgicc-init.h"
// #include "mysql-init.h"

// Use required namespaces
using namespace std;
using namespace cgiccInit;
// using namespace mysqlInit;

// Entry function
int main(int argc, char *argv[]) {
    Cgicc cgi;
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Test")) << endl;
    cout << body();
    cout << body() << html();
/*
    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Register")) << endl;
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

*/
    return 0;
}
