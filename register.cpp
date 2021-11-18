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
    CgiccInit cgi = CgiccInit();

    // Required response data
    cout << HTTPHTMLHeader() << endl;
    cout << html() << head(title("Register")) << endl;
    cout << body();

    // Form for registration
    auto registerForm = form();
    registerForm.set("method", "post"); // Submit as post request

    // Username input
    registerForm.add(cgicc::div()
        .add(span("Username: "))
        .add(input()
            .set("type", "text")
            .set("name", "username")
            .set("placeholder", "Username")
        )
    );

    // Email input
    registerForm.add(cgicc::div()
        .add(span("Email: "))
        .add(input()
            .set("type", "text")
            .set("name", "email")
            .set("placeholder", "Email")
        )
    );

    // Password input
    registerForm.add(cgicc::div()
        .add(span("Password: "))
        .add(input()
            .set("type", "password")
            .set("name", "password")
            .set("placeholder", "Password")
        )
    );

    // Skill input
    registerForm.add(cgicc::div()
        .add(span("Skill: "))
        .add(input()
            .set("type", "text")
            .set("name", "skill")
            .set("placeholder", "Skill")
        )
    );

    // Role dropdown
    registerForm.add(cgicc::div()
        .add(span("Role: "))
        .add(cgicc::select()
            .set("name", "role")
            // .add(option("Select a role...")
            //     .set("hidden", "")
            //     .set("disabled", "")
            //     .set("selected", "")
            //     .set("value", "")
            // )
            .add(option("Applicant")
                .set("value", "Applicant")
            )
            .add(option("Company")
                .set("value", "Company")
            )
            .add(option("Administrator")
                .set("value", "Administrator")
            )
        )
    );

    // Register button
    registerForm.add(input()
        .set("type", "submit")
        .set("value", "Register")
    );

    // Login link
    registerForm.add(a("Login")
        .set("href", "./login.cgi")
    );

    // Display register form
    cout << registerForm;

    // End of response
    cout << body() << html();

    return 0;
}
