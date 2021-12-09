// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised CGICC header
#include "Utils/CGICCInit.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Component headers
#include "Components/ApprovedApplicantsList.hpp"
#include "Components/AuthenticationRedirect.hpp"
#include "Components/LogOutButton.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Users.hpp"
#include "Utils/Skills.hpp"
#include "Utils/Roles.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Company CGI page class structure
class CompanyCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;
};

// On GET request method
void CompanyCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect(COMPANY_ROLE_NAME);
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

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
        return;
    }

    // Attempt to get session from token
    SessionResult sessionResult = Authentication::getSessionByToken(sessionCookie.getValue());
    if (!sessionResult.getSuccess()) {
        return;
    }

    // Get user of session
    User user = sessionResult.session->getUser();

    // Attempt to get user's skill search
    SkillSearchResult skillSearchResult = Skills::getUserSkillSearch(user);
    if (!skillSearchResult.getSuccess()) {
        return;
    }

    // Get user's skill
    Skill skill = skillSearchResult.skillSearch->getSkill();
    string skillName = skill.getName();

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Company")) << endl;
    os << body();

    // Page heading
    os << h1("Company");

    // Page description
    os << p("View applicants with the skill you're looking for.");

    // Log out from session button
    os << LogOutButton(false);

    // Log out from all sessions button
    os << LogOutButton(true);

    // Approved text placeholder
    string approvedText;

    // Set approved text
    if (skillSearchResult.skillSearch->getApprovedState()) {
        // When skill search is approved
        approvedText = "Your search for '" + skillName + "' has been approved!";
    } else {
        // When skill search is not approved
        approvedText = "Your search for '" + skillName + "' has not yet been approved!";
    }

    // Display approved text
    os << p(approvedText);

    // Display sub-heading
    os << h2("Applicants with the skill you're looking for:");

    // Display list of approved applicants
    os << ApprovedApplicantsList(skillName);

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << CompanyCGIPage();

    return 0;
}
