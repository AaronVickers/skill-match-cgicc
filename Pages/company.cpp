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
#include "Components/ApprovedApplicantsList.hpp"
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

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
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Company");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // TODO: Get company's skill

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Company")) << endl;
    os << body();

    // Page heading
    os << h1("Company");

    // TODO: Use company's skill
    // Display list of approved applicants
    os << ApprovedApplicantsList("test");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << CompanyCGIPage();

    return 0;
}
