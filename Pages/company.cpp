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

// Company CGI page class structure
class CompanyCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;
};

// On GET request method
void CompanyCGIPage::onGET(ostream &os) const {
    // TODO: Redirect if not logged in as company
    // TODO: Show list of approved applicants with company's skill

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Company")) << endl;
    os << body();

    // Temporary response
    os << h1("Company");
    os << p("Temporary response");

    // End of response
    os << body() << html();
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << CompanyCGIPage();

    return 0;
}
