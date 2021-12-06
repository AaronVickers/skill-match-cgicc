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
#include "Components/ApproveCompaniesForm.hpp"
#include "Components/AuthenticationRedirect.hpp"
#include "Components/LogOutButton.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"
#include "Utils/Skills.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Approve companies CGI page class structure
class ApproveCompaniesCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void ApproveCompaniesCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Administrator");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Approve Companies")) << endl;
    os << body();

    // Page heading
    os << h1("Approve Companies");

    // Log out button
    os << LogOutButton();

    // Display form to approve companies
    os << ApproveCompaniesForm();

    // Back link
    os << cgicc::a("Back")
        .set("href", "./admin.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void ApproveCompaniesCGIPage::onPOST(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Administrator");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Approved and failed counts
    int approvedCount = 0;
    int failedCount = 0;

    // Get submitted form elements
    const vector<FormEntry> formElements = CGICCInit::cgi->getElements();

    // Loop over all form elements
    for (auto &formEntry: formElements) {
        // Check if name of element is approve
        if (formEntry.getName().compare("approve") == 0) {
            // Attempt to approve skill search
            try {
                // Get skill search ID integer from value of element
                int skillSearchId = stoi(formEntry.getValue());

                // Get skill search by skill search ID
                SkillSearch skillSearch = SkillSearch(skillSearchId);

                // Approve skill search
                skillSearch.setApprovedState(true);

                // Increase approved count
                approvedCount++;
            } catch(exception &e) {
                // Increase failed count
                failedCount++;
            }
        }
    }

    // Redirect location string with approved and failed counts
    string redirectLocation = "./approveCompanies.cgi?approved=" + to_string(approvedCount) + "&failed=" + to_string(failedCount);

    // Redirect to approve companies page
    os << HTTPRedirectHeader(redirectLocation, false) << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << ApproveCompaniesCGIPage();

    return 0;
}
