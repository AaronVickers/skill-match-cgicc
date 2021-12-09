// Header guard
#ifndef CODE_FORM_CGI_COMPONENT_H_
#define CODE_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Code form CGI component class structure
class CodeForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
