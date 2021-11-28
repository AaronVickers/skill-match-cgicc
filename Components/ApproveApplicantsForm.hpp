// Header guard
#ifndef APPROVE_APPLICANTS_FORM_CGI_COMPONENT_H_
#define APPROVE_APPLICANTS_FORM_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Required headers
#include <string>

// Approve applicants form CGI component class structure
class ApproveApplicantsForm: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
