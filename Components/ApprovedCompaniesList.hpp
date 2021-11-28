// Header guard
#ifndef APPROVED_COMPANIES_LIST_CGI_COMPONENT_H_
#define APPROVED_COMPANIES_LIST_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Required headers
#include <string>

// Approved companies list CGI component class structure
class ApprovedCompaniesList: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;

    // Skill to search for
    std::string skillName;
public:
    // Constructor
    ApprovedCompaniesList(std::string _skillName);
};

// End of header guard
#endif
