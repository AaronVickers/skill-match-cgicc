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
#include "Components/ApproveCompaniesForm.hpp"

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

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void AdminCGIPage::onGET(ostream &os) const {
    // TODO: Redirect if not logged in as admin

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Approve Companies")) << endl;
    os << body();

    // Page heading
    os << h1("Approve Companies");

    // Display form to approve companies
    os << ApproveCompaniesForm();

    // Back link
    os << cgicc::a("Back")
        .set("href", "./admin.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void AdminCGIPage::onPOST(ostream &os) const {
    // TODO: Verify that user is admin
    // TODO: Approve supplied companies
    
    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Approve Companies")) << endl;
    os << body();

    // Temporary response
    os << h1("Approve Companies");
    os << p("POST request not implemented yet.");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << AdminCGIPage();

    return 0;
}
