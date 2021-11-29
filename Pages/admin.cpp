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
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Admin CGI page class structure
class AdminCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;
};

// On GET request method
void AdminCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect("Administrator");
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Admin")) << endl;
    os << body();

    // Page heading
    os << h1("Admin");

    // Approve applicants link
    os << a("Approve Applicants")
        .set("href", "./approveApplicants.cgi");

    // Line break between links
    os << br();

    // Approve companies link
    os << a("Approve Companies")
        .set("href", "./approveCompanies.cgi");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << AdminCGIPage();

    return 0;
}
