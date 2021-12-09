// Include header file
#include "Components/CodeForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>

// Render method
void CodeForm::render(std::ostream &os) const {
    // Form for code input
    auto codeForm = cgicc::form();
    codeForm.set("method", "post"); // Submit as post request

    // Code input
    codeForm.add(cgicc::div()
        .add(cgicc::span("Code: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "code")
            .set("placeholder", "Code")
        )
    );

    // Submit button
    codeForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Submit")
    );

    // Render code form
    os << codeForm;
}