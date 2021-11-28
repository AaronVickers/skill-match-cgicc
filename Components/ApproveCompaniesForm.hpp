// Header guard
#ifndef APPROVE_COMPANIES_FORM_CGI_COMPONENT_H_
#define APPROVE_COMPANIES_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Required headers
#include <string>

// Approve companies form CGI component class structure
class ApproveCompaniesForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
