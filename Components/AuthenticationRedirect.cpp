// Include header file
#include "Components/AuthenticationRedirect.hpp"

// Initialised CGICC header
#include "Utils/CGICCInit.hpp"

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
        std::vector<cgicc::HTTPCookie> cookies = CGICCInit::env->getCookieList();

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

        // Handle inactive session
        if (!sessionResult.session->getActive()) {
            redirectRequired = false;

            return;
        }

        // Handle expired session
        if (sessionResult.session->isExpired()) {
            redirectRequired = false;

            return;
        }

        // Get user of session
        User user = sessionResult.session->getUser();

        // Handle locked account
        if (user.getLocked()) {
            redirectRequired = false;

            return;
        }

        // Set redirect required
        redirectRequired = true;

        // Get role of user
        Role role = user.getRole();
        std::string roleName = role.getName();

        // Get corresponding page for user
        if (roleName.compare(ADMINISTRATOR_ROLE_NAME) == 0) {
            redirectLocation = "./admin.cgi";
        } else if (roleName.compare(APPLICANT_ROLE_NAME) == 0) {
            redirectLocation = "./applicant.cgi";
        } else if (roleName.compare(COMPANY_ROLE_NAME) == 0) {
            redirectLocation = "./company.cgi";
        }
    } else if (pageType == TFA_PAGE) {
        // Get list of cookies
        std::vector<cgicc::HTTPCookie> cookies = CGICCInit::env->getCookieList();

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

        // Attempt to get 2FA session from 2FA cookie
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

        // Handle expired 2FA session
        if (tfaResult.tfaSession->isExpired()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=expired_tfa_session";

            return;
        }

        // Set redirect not required
        redirectRequired = false;
    } else if (pageType == TOTP_PAGE) {
        // Get list of cookies
        std::vector<cgicc::HTTPCookie> cookies = CGICCInit::env->getCookieList();

        // Placeholder for TOTP cookie and found status
        bool totpCookieFound = false;
        cgicc::HTTPCookie totpCookie;

        // Find TOTP cookie
        for (auto &cookie: cookies) {
            if (cookie.getName().compare("TOTP_TOKEN") == 0) {
                totpCookieFound = true;
                totpCookie = cookie;

                break;
            }
        }

        // Check if TOTP cookie was found
        if (!totpCookieFound) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_totp_session";

            return;
        }

        // Attempt to get TOTP session from TOTP cookie
        TOTPResult totpResult = Authentication::getTOTPByToken(totpCookie.getValue());
        if (!totpResult.getSuccess()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_totp_session";

            return;
        }

        // Handle already authenticated TOTP session
        if (totpResult.totpSession->getAuthenticated()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=invalid_totp_session";

            return;
        }

        // Handle expired TOTP session
        if (totpResult.totpSession->isExpired()) {
            redirectRequired = true;
            redirectLocation = "./login.cgi?error=expired_totp_session";

            return;
        }

        // Set redirect not required
        redirectRequired = false;
    }
}

// Constructor for required role
AuthenticationRedirect::AuthenticationRedirect(std::string requiredRole) {
    // Get list of cookies
    std::vector<cgicc::HTTPCookie> cookies = CGICCInit::env->getCookieList();

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

    // Handle inactive session
    if (!sessionResult.session->getActive()) {
        redirectRequired = true;
        redirectLocation = "./login.cgi?error=inactive_session";

        return;
    }

    // Handle expired session
    if (sessionResult.session->isExpired()) {
        redirectRequired = true;
        redirectLocation = "./login.cgi?error=expired_session";

        return;
    }

    // Get user of session
    User user = sessionResult.session->getUser();

    // Handle locked account
    if (user.getLocked()) {
        redirectRequired = true;
        redirectLocation = "./login.cgi?error=account_locked";

        return;
    }

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
    if (roleName.compare(ADMINISTRATOR_ROLE_NAME) == 0) {
        redirectLocation = "./admin.cgi";
    } else if (roleName.compare(APPLICANT_ROLE_NAME) == 0) {
        redirectLocation = "./applicant.cgi";
    } else if (roleName.compare(COMPANY_ROLE_NAME) == 0) {
        redirectLocation = "./company.cgi";
    }
}