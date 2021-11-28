// Header guard
#ifndef APPROVED_APPLICANTS_LIST_CGI_COMPONENT_H_
#define APPROVED_APPLICANTS_LIST_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Required headers
#include <string>

// Approved applicants list CGI component class structure
class ApprovedApplicantsList: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;

    // Skill to search for
    std::string skillName;
public:
    // Constructor
    ApprovedApplicantsList(std::string _skillName);
};

// End of header guard
#endif
