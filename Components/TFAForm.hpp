// Header guard
#ifndef TFA_FORM_CGI_COMPONENT_H_
#define TFA_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// 2FA form CGI component class structure
class TFAForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
