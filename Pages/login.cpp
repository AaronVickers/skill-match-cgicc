// Define environment constants
#define SMTP_SERVER "smtp.example.com"
#define SMTP_TARGET_PORT 587
#define SMTP_LOGIN "username@example.com"
#define SMTP_PASSWORD "SMTP_PASSWORD"
#define TFA_SENDER_NAME "Skill Match"
#define TFA_SENDER_EMAIL "username@example.com"

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

// POCO headers
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SMTPClientSession.h"

// Component headers
#include "Components/LoginForm.hpp"
#include "Components/AuthenticationRedirect.hpp"

// Required headers
#include <ostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;
using namespace cgicc;

// Login CGI page class structure
class LoginCGIPage: public CGIPage {
private:
    // On GET request method
    void onGET(std::ostream &os) const;

    // On POST request method
    void onPOST(std::ostream &os) const;
};

// On GET request method
void LoginCGIPage::onGET(ostream &os) const {
    // Check if redirect is required
    AuthenticationRedirect authenticationRedirect = AuthenticationRedirect(UNAUTHENTICATED_PAGE);
    if (authenticationRedirect.getRedirectRequired()) {
        // Send redirect if required
        os << authenticationRedirect;

        return;
    }

    // Required response data
    os << HTTPHTMLHeader() << endl;
    os << html() << head(title("Login")) << endl;
    os << body();

    // Page heading
    os << h1("Login");

    // Page description
    os << p("Log in to your account.");

    // Display login form
    os << LoginForm();

    // Register link
    os << cgicc::a("Register")
        .set("href", "./register.cgi");

    // End of response
    os << body() << html();
}

// On POST request method
void LoginCGIPage::onPOST(ostream &os) const {
    // Get form data
    string username = CGICCInit::cgi->getElement("username")->getValue();
    string password = CGICCInit::cgi->getElement("password")->getValue();

    // Attempt login
    LoginResult loginResult = Authentication::login(username, password);

    // Redirect location string
    string redirectLocation;

    // Handle error
    if (!loginResult.getSuccess()) {
        // Redirect to login page with error message
        redirectLocation = "./login.cgi?error=" + loginResult.getErrorMsg();
        os << HTTPRedirectHeader(redirectLocation, false) << endl;

        return;
    }

    // Get logged in user
    User user = loginResult.tfaSession->getUser();

    // Create 2FA code message
    Poco::Net::MailMessage tfaCodeMsg;
    // Set 2FA code message details
    tfaCodeMsg.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, user.getEmail(), user.getUsername()));
    tfaCodeMsg.setSender(TFA_SENDER_NAME " <" TFA_SENDER_EMAIL ">");
    tfaCodeMsg.setSubject("2FA Code");
    tfaCodeMsg.setContent(loginResult.tfaSession->getCode());

    // Create SMTP client instance with specified SMTP server
    Poco::Net::SMTPClientSession smtpClient(SMTP_SERVER, SMTP_TARGET_PORT);
    // Authenticate and send 2FA code message
    smtpClient.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, SMTP_LOGIN, SMTP_PASSWORD); 
    smtpClient.sendMessage(tfaCodeMsg);
    smtpClient.close();

    // Create 2FA cookie
    string tfaTokenValue = loginResult.tfaSession->getToken() + "; HttpOnly";
    HTTPCookie tfaCookie = HTTPCookie("TFA_TOKEN", tfaTokenValue);

    // Redirect to 2FA page with 2FA cookie
    redirectLocation = "./tfa.cgi";
    os << HTTPRedirectHeader(redirectLocation, false)
        .setCookie(tfaCookie)
        << endl;
}

// Entry function
int main(int argc, char *argv[]) {
    // Display page
    cout << LoginCGIPage();

    return 0;
}
