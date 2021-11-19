// Initialised headers
#include "Utils/CgiccInit.hpp"
#include "Utils/MySqlInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

// Required headers
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

void onGET(CgiccInit &cgi) {
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
    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Two-Factor Authentication")) << endl;
    cout << body();

    cout << p("Two-Factor Authentication POST Request");

    // End of response
    cout << body() << html();
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
