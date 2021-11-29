// Header guard
#ifndef AUTHENTICATION_REDIRECT_CGI_COMPONENT_H_
#define AUTHENTICATION_REDIRECT_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

enum PageType {
    UNAUTHENTICATED_PAGE,
    TFA_PAGE
};

// 2FA form CGI component class structure
class AuthenticationRedirect: public CGIComponent {
private:
    // Whether redirect is required
    bool redirectRequired;

    // Redirect location
    std::string redirectLocation;

    // Render method
    void render(std::ostream &os) const;
public:
    bool getRedirectRequired();

    // Constructor for pages that don't require authentication
    AuthenticationRedirect(PageType pageType);

    // Constructor for pages that require authentication
    AuthenticationRedirect(std::string requiredRole);
};

// End of header guard
#endif
