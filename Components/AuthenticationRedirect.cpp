// Include header file
#include "Components/AuthenticationRedirect.hpp"

// CGICC headers
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/Roles.hpp"
#include "Utils/Sessions.hpp"

// Render method
void AuthenticationRedirect::render(std::ostream &os) const {
    // Check if redirect is required
    if (!redirectRequired) {
        return;
    }

    // Render redirect header
    os << cgicc::HTTPRedirectHeader(redirectLocation, false) << std::endl;
}

bool AuthenticationRedirect::getRedirectRequired() {
    // Return redirect required
    return redirectRequired;
}

// Constructor for no required role
AuthenticationRedirect::AuthenticationRedirect(PageType pageType) {
    if (pageType == UNAUTHENTICATED_PAGE) {
        // Get list of cookies
        std::vector<cgicc::HTTPCookie> cookies = env.getCookieList();

        // Placeholder for session cookie and found status
        bool sessionCookieFound = false;
        cgicc::HTTPCookie sessionCookie;

        // Find session cookie
        for (auto &cookie: cookies) {
            if (cookie.getName().compare("SESSION_TOKEN") == 0) {
                sessionCookieFound = true;
                sessionCookie = cookie;

                break;
            }
        }

        // Check if session cookie doesn't exist
        if (!sessionCookieFound) {
            redirectRequired = false;

            return;
        }

        // Attempt to get session from token
        SessionResult sessionResult = Authentication::getSessionByToken(sessionCookie.getValue());
        if (!sessionResult.getSuccess()) {
            redirectRequired = false;

            return;
        }

        // TODO: Handle expired session

        // Set redirect required
        redirectRequired = true;

        // Get user of session
        User user = sessionResult.session->getUser();

        // Get role of user
        Role role = user.getRole();
        std::string roleName = role.getName();

        // Get corresponding page for user
        if (roleName.compare("Administrator") == 0) {
            redirectLocation = "./admin.cgi";
        } else if (roleName.compare("Company") == 0) {
            redirectLocation = "./company.cgi";
        } else if (roleName.compare("Applicant") == 0) {
            redirectLocation = "./applicant.cgi";
        }
    } else if (pageType == TFA_PAGE) {
        // Get list of cookies
        std::vector<cgicc::HTTPCookie> cookies = env.getCookieList();

        // Placeholder for 2FA cookie and found status
        bool tfaCookieFound = false;
        cgicc::HTTPCookie tfaCookie;

        // Find 2FA cookie
        for (auto &cookie: cookies) {
            if (cookie.getName().compare("TFA_TOKEN") == 0) {
                tfaCookieFound = true;
                tfaCookie = cookie;

                break;
            }
        }

        // Check if 2FA cookie was found
        if (!tfaCookieFound) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_tfa_session";

            return;
        }

        // Attempt to get 2FA session from
        TFAResult tfaResult = Authentication::getTFAByToken(tfaCookie.getValue());
        if (!tfaResult.getSuccess()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_tfa_session";

            return;
        }

        // Handle already authenticated 2FA session
        if (tfaResult.tfaSession->getAuthenticated()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_tfa_session";

            return;
        }

        // TODO: Handle expired 2FA session

        // Set redirect not required
        redirectRequired = false;
    }
}

// Constructor for required role
AuthenticationRedirect::AuthenticationRedirect(std::string requiredRole) {
    // Get list of cookies
    std::vector<cgicc::HTTPCookie> cookies = env.getCookieList();

    // Placeholder for session cookie and found status
    bool sessionCookieFound = false;
    cgicc::HTTPCookie sessionCookie;

    // Find session cookie
    for (auto &cookie: cookies) {
        if (cookie.getName().compare("SESSION_TOKEN") == 0) {
            sessionCookieFound = true;
            sessionCookie = cookie;

            break;
        }
    }

    // Check if session cookie doesn't exist
    if (!sessionCookieFound) {
        redirectRequired = true;
        redirectLocation = "./login.cgi?error=invalid_session";

        return;
    }

    // Attempt to get session from token
    SessionResult sessionResult = Authentication::getSessionByToken(sessionCookie.getValue());
    if (!sessionResult.getSuccess()) {
        redirectRequired = true;
        redirectLocation = "./login.cgi?error=invalid_session";

        return;
    }

    // Get user of session
    User user = sessionResult.session->getUser();

    // Get role of user
    Role role = user.getRole();
    std::string roleName = role.getName();

    // Check if role matches required role
    if (roleName.compare(requiredRole) == 0) {
        redirectRequired = false;

        return;
    }

    // Set redirect required
    redirectRequired = true;

    // Get corresponding page for user
    if (roleName.compare("Administrator") == 0) {
        redirectLocation = "./admin.cgi";
    } else if (roleName.compare("Company") == 0) {
        redirectLocation = "./company.cgi";
    } else if (roleName.compare("Applicant") == 0) {
        redirectLocation = "./applicant.cgi";
    }
}