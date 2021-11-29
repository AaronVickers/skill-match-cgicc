// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

// Component headers
#include "Components/ApproveApplicantsForm.hpp"
#include "Components/AuthenticationRedirect.hpp"
#include "Components/LogOutButton.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Approve applicants CGI page class structure
class ApproveApplicantsCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void ApproveApplicantsCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Administrator");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Approve Applicants")) << endl;
    os << body();

    // Page heading
    os << h1("Approve Applicants");

    // Log out button
    os << LogOutButton();

    // Display form to approve applicants
    os << ApproveApplicantsForm();

    // Back link
    os << cgicc::a("Back")
        .set("href", "./admin.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void ApproveApplicantsCGIPage::onPOST(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Administrator");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // TODO: Approve supplied applicants

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Approve Applicants")) << endl;
    os << body();

    // Temporary response
    os << h1("Approve Applicants");
    os << p("POST request not implemented yet.");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << ApproveApplicantsCGIPage();

    return 0;
}
