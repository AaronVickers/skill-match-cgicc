// Include header file
#include "Components/ApprovedApplicantsList.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>
#include "Utils/Users.hpp"
#include "Utils/Skills.hpp"

// Constructor
ApprovedApplicantsList::ApprovedApplicantsList(std::string _skillName) {
    skillName = _skillName;
}

// Render method
void ApprovedApplicantsList::render(std::ostream &os) const {
    // Get approved applicants with skill
    UsersResult approvedApplicants = Skills::getApprovedApplicantsBySkill(skillName);

    // Handle error getting list of applicants
    if (!approvedApplicants.getSuccess()) {
        os << cgicc::p("Failed to get list of applicants.");

        return;
    }

    // Handle empty list
    if (approvedApplicants.users.empty()) {
        os << cgicc::p("No applicants found.");

        return;
    }

    // List of approved applicants
    auto ApprovedApplicantsList = cgicc::ul();

    // Add approved applicants to list
    for (auto &applicant: approvedApplicants.users) {
        ApprovedApplicantsList.add(cgicc::li(applicant.getUsername()));
    }

    // Render list of approved applicants
    os << ApprovedApplicantsList;
}