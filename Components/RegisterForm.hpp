// Header guard
#ifndef REGISTER_FORM_CGI_COMPONENT_H_
#define REGISTER_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Register form CGI component class structure
class RegisterForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
