// Define environment constants
#define MY_EMAIL "username@example.com"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// Required headers
#include <iostream>
#include "Utils/Authentication.hpp"
#include "Utils/Roles.hpp"

// Use required namespaces
using namespace std;

// Entry function
int main(int argc, char *argv[]) {
    // Notify user that operation has started
    cout << "Populating database..." << endl;

    // Register admin account
    Authentication::registerAccount("Admin", MY_EMAIL, "admin1234", "Admin", ADMINISTRATOR_ROLE_NAME);

    // Register applicant accounts
    Authentication::registerAccount("Appl1", MY_EMAIL, "appl1234", "Programming", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("appl2", MY_EMAIL, "appl1234", "Accounting", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("APPL3", MY_EMAIL, "appl1234", "Cooking", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("Appl4", MY_EMAIL, "appl1234", "programming", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("appl5", MY_EMAIL, "appl1234", "PROGRAMMING", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("APPL6", MY_EMAIL, "appl1234", "Designing", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("Appl7", MY_EMAIL, "appl1234", "accounting", APPLICANT_ROLE_NAME);
    Authentication::registerAccount("appl8", MY_EMAIL, "appl1234", "TESTING", APPLICANT_ROLE_NAME);

    // Register applicant accounts
    Authentication::registerAccount("Comp1", MY_EMAIL, "comp1234", "PROgramming", COMPANY_ROLE_NAME);
    Authentication::registerAccount("comp2", MY_EMAIL, "comp1234", "cleaning", COMPANY_ROLE_NAME);
    Authentication::registerAccount("COMP3", MY_EMAIL, "comp1234", "Eating", COMPANY_ROLE_NAME);
    Authentication::registerAccount("Comp4", MY_EMAIL, "comp1234", "PrOgRaMmInG", COMPANY_ROLE_NAME);
    Authentication::registerAccount("comp5", MY_EMAIL, "comp1234", "Accounting", COMPANY_ROLE_NAME);
    Authentication::registerAccount("COMP6", MY_EMAIL, "comp1234", "Marketing", COMPANY_ROLE_NAME);
    Authentication::registerAccount("Comp7", MY_EMAIL, "comp1234", "marketING", COMPANY_ROLE_NAME);
    Authentication::registerAccount("comp8", MY_EMAIL, "comp1234", "marketing", COMPANY_ROLE_NAME);

    // Notify user that operation was completed
    cout << "Operation complete." << endl;

    return 0;
}
