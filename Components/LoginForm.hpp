// Header guard
#ifndef LOGIN_FORM_CGI_COMPONENT_H_
#define LOGIN_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Login form CGI component class structure
class LoginForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
