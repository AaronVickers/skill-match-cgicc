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
    logOutForm.set("method", "post"); // Submit as post request

    // Log out button
    auto logOutButton = cgicc::input();
    logOutButton.set("type", "submit");

    // Handle button type
    if (fromAllSessions) {
        // Submit to logout.cgi with all parameter
        logOutForm.set("action", "./logout.cgi?fromAllSessions=1");

        // Set log out button text
        logOutButton.set("value", "Log Out Everywhere");
    } else {
        // Submit to logout.cgi
        logOutForm.set("action", "./logout.cgi");

        // Set log out button text
        logOutButton.set("value", "Log Out");
    }

    // Add log out button to form
    logOutForm.add(logOutButton);

    // Render log out form
    os << logOutForm;
}

LogOutButton::LogOutButton(bool _fromAllSessions) {
    // Set button type
    fromAllSessions = _fromAllSessions;
}