// Include header file
#include "Components/ApproveCompaniesForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>
#include "Utils/Users.hpp"
#include "Utils/Skills.hpp"

// Render method
void ApproveCompaniesForm::render(std::ostream &os) const {
    // Get unapproved company skill searches
    SkillSearchesResult companySkillSearches = Skills::getUnapprovedCompanySkillSearches();

    // Handle error getting skill searches
    if (!companySkillSearches.getSuccess()) {
        os << cgicc::p("Failed to get list of unapproved company skill searches.");

        return;
    }

    // Handle empty list
    if (companySkillSearches.skillSearches.empty()) {
        os << cgicc::p("No company skill searches to be approved.");

        return;
    }

    // Form to approve companies
    auto approveCompaniesForm = cgicc::form();
    approveCompaniesForm.set("method", "post"); // Submit as post request

    // Table to list company skill searches
    auto approveCompaniesTable = cgicc::table();

    // Add table header
    approveCompaniesTable.add(cgicc::thead()
        .add(cgicc::tr()
            .add(cgicc::th("Company"))
            .add(cgicc::th("Skill"))
            .add(cgicc::th("Approve"))
        )
    );

    // Create table body
    auto approveCompaniesTableBody = cgicc::tbody();

    // Add company skill searches to list
    for (auto &skillSearch: companySkillSearches.skillSearches) {
        // Get company and skill from skill search
        User company = skillSearch.getUser();
        Skill skill = skillSearch.getSkill();

        // Checkbox to approve company
        auto approveCheckbox = cgicc::input();
        approveCheckbox.set("type", "checkbox")
            .set("name", "approve")
            .set("value", std::to_string(skillSearch.getSkillSearchId()));

        // Add data to table body
        approveCompaniesTableBody.add(cgicc::tr()
            .add(cgicc::td(company.getUsername()))
            .add(cgicc::td(skill.getName()))
            .add(cgicc::td().add(approveCheckbox))
        );
    }

    // Add table body to table
    approveCompaniesTable.add(approveCompaniesTableBody);

    // Add table to form
    approveCompaniesForm.add(approveCompaniesTable);

    // Submit button
    approveCompaniesForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Submit")
    );

    // Render list of approved companies
    os << approveCompaniesForm;
}
