// Include header file
#include "Components/LoginForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>

// Render method
void LoginForm::render(std::ostream &os) const {
    // Form for login
    auto loginForm = cgicc::form();
    loginForm.set("method", "post"); // Submit as post request

    // Username input
    loginForm.add(cgicc::div()
        .add(cgicc::span("Username: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "username")
            .set("placeholder", "Username")
        )
    );

    // Password input
    loginForm.add(cgicc::div()
        .add(cgicc::span("Password: "))
        .add(cgicc::input()
            .set("type", "password")
            .set("name", "password")
            .set("placeholder", "Password")
        )
    );

    // Login button
    loginForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Login")
    );

    // Render login form
    os << loginForm;
}