// CGI page header
#include "Utils/CGIPage.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// CGICC headers
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPRedirectHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPCookie.h"

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
    // TODO: Show list of approved companies with applicant's skill

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Applicant")) << endl;
    os << body();

    // Temporary response
    os << h1("Applicant");
    os << p("Temporary response");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << ApplicantCGIPage();

    return 0;
}
