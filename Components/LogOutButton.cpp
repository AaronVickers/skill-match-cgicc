// Include header file
#include "Components/LogOutButton.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>

// Render method
void LogOutButton::render(std::ostream &os) const {
    // Form for log out
    auto logOutForm = cgicc::form();
    logOutForm.set("action", "./logout.cgi"); // Submit to logout.cgi
    logOutForm.set("method", "post"); // Submit as post request

    // Log out button
    logOutForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Log Out")
    );

    // Render log out form
    os << logOutForm;
}