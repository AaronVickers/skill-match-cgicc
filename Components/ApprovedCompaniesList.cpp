// Include header file
#include "Components/ApprovedCompaniesList.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>
#include "Utils/Users.hpp"
#include "Utils/Skills.hpp"

// Constructor
ApprovedCompaniesList::ApprovedCompaniesList(std::string _skillName) {
    skillName = _skillName;
}

// Render method
void ApprovedCompaniesList::render(std::ostream &os) const {
    // Get approved companies with skill
    UsersResult approvedCompanies = Skills::getApprovedCompaniesBySkill(skillName);

    // Handle error getting list of companies
    if (!approvedCompanies.getSuccess()) {
        os << cgicc::p("Failed to get list of companies.");

        return;
    }

    // Handle empty list
    if (approvedCompanies.users.empty()) {
        os << cgicc::p("No companies found.");

        return;
    }

    // List of approved companies
    auto approvedCompaniesList = cgicc::ul();

    // Add approved companies to list
    for (auto &company: approvedCompanies.users) {
        approvedCompaniesList.add(cgicc::li(company.getUsername()));
    }

    // Render list of approved companies
    os << approvedCompaniesList;
}