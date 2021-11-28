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
#include "Components/ApprovedCompaniesList.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Applicant CGI page class structure
class ApplicantCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;
};

// On GET request method
void ApplicantCGIPage::onGET(ostream &os) const {
    // TODO: Redirect if not logged in as applicant
    // TODO: Get applicant's skill

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Applicant")) << endl;
    os << body();

    // TODO: Use applicant's skill
    // Display list of approved companies
    os << ApprovedCompaniesList("test");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << ApplicantCGIPage();

    return 0;
}
