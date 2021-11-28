// Include header file
#include "Components/ApproveApplicantsForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>
#include "Utils/Users.hpp"
#include "Utils/Skills.hpp"

// Render method
void ApproveApplicantsForm::render(std::ostream &os) const {
    // Get unapproved applicant skill searches
    SkillSearchesResult applicantSkillSearches = Skills::getUnapprovedApplicantSkillSearches();

    // Handle error getting skill searches
    if (!applicantSkillSearches.getSuccess()) {
        os << cgicc::p("Failed to get list of unapproved applicant skill searches.");

        return;
    }

    // Handle empty list
    if (applicantSkillSearches.skillSearches.empty()) {
        os << cgicc::p("No applicant skill searches to be approved.");

        return;
    }

    // Form to approve applicants
    auto approveApplicantsForm = cgicc::form();
    approveApplicantsForm.set("method", "post"); // Submit as post request

    // Table to list applicant skill searches
    auto approveApplicantsTable = cgicc::table();

    // Add table header
    approveApplicantsTable.add(cgicc::thead()
        .add(cgicc::tr()
            .add(cgicc::th("Applicant"))
            .add(cgicc::th("Skill"))
            .add(cgicc::th("Approve"))
        )
    );

    // Create table body
    auto approveApplicantsTableBody = cgicc::tbody();

    // Add applicant skill searches to list
    for (auto &skillSearch: applicantSkillSearches.skillSearches) {
        // Get applicant and skill from skill search
        User applicant = skillSearch.getUser();
        Skill skill = skillSearch.getSkill();

        // Checkbox to approve applicant
        auto approveCheckbox = cgicc::input();
        approveCheckbox.set("type", "checkbox")
            .set("name", "approve")
            .set("value", std::to_string(skillSearch.getSkillSearchId()));

        // Add data to table body
        approveApplicantsTableBody.add(cgicc::tr()
            .add(cgicc::td(applicant.getUsername()))
            .add(cgicc::td(skill.getName()))
            .add(cgicc::td().add(approveCheckbox))
        );
    }

    // Add table body to table
    approveApplicantsTable.add(approveApplicantsTableBody);

    // Add table to form
    approveApplicantsForm.add(approveApplicantsTable);

    // Submit button
    approveApplicantsForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Submit")
    );

    // Render list of approved applicants
    os << approveApplicantsForm;
}
