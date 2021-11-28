// Include header file
#include "Components/TFAForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>

// Render method
void TFAForm::render(std::ostream &os) const {
    // Form for 2FA
    auto TFAForm = cgicc::form();
    TFAForm.set("method", "post"); // Submit as post request

    // Code input
    TFAForm.add(cgicc::div()
        .add(cgicc::span("Code: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "code")
            .set("placeholder", "Code")
        )
    );

    // Submit button
    TFAForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Submit")
    );

    // Login link
    TFAForm.add(cgicc::a("Login")
        .set("href", "./login.cgi")
    );

    // Render 2FA form
    os << TFAForm;
}