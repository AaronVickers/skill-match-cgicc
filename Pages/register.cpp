// Initialised headers
#include "Utils/CgiccInit.hpp"
#include "Utils/MariaDBInit.hpp"

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
    // TODO: Redirect if logged in
    
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

    // Create roles selection
    auto roleSelect = cgicc::select();
    roleSelect.set("name", "role");

    // Add admin to roles selection if admin doesn't exist
    if (!Users::doesAdminExist()) {
        roleSelect.add(option("Administrator")
            .set("value", "Administrator")
        );
    }

    // Add applicant to roles selection
    roleSelect.add(option("Applicant")
        .set("value", "Applicant")
    );

    // Add company to roles selection
    roleSelect.add(option("Company")
        .set("value", "Company")
    );

    // Role dropdown
    registerForm.add(cgicc::div()
        .add(span("Role: "))
        .add(roleSelect)
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
}

void onPOST(CgiccInit &cgi) {
    // Get form data
    string username = cgi.cgi.getElement("username")->getValue();
    string email = cgi.cgi.getElement("email")->getValue();
    string password = cgi.cgi.getElement("password")->getValue();
    string skill = cgi.cgi.getElement("skill")->getValue();
    string role = cgi.cgi.getElement("role")->getValue();

    // Attempt to register
    RegisterResult registerResult = Authentication::registerAccount(username, email, password, skill, role);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!registerResult.getSuccess()) {
        // Redirect to register page with error message
        redirectLocation = "./register.cgi?error=" + registerResult.getErrorMsg();
        cout << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Redirect to login page with message
    redirectLocation = "./login.cgi?message=registered";
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
