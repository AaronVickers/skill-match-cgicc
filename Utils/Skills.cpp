// Include header file
#include "Utils/Skills.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>

int Skill::getSkillId() {
    // Return skill ID
    return skillId;
}

std::string Skill::getName() {
    // Return name
    return name;
}

Skill::Skill(int _skillId) {
    skillId = _skillId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Skills WHERE SkillId=?");

    // Execute query
    pstmt->setInt(1, skillId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get skill name from row
    name = res->getString("Name").c_str();

    // Delete result from memory
    delete res;
}

Skill::Skill(std::string _name) {
    // TODO: Make sure skill doesn't exist
    // TODO: Create new skill
}

Skill::Skill(int _skillId, std::string _name) {
    skillId = _skillId;
    name = _name;
}

int SkillSearch::getSkillSearchId() {
    // Return skill search ID
    return skillSearchId;
}

int SkillSearch::getSkillId() {
    // Return skill ID
    return skillId;
}

Skill SkillSearch::getSkill() {
    // Get skill
    Skill skill = Skill(skillId);

    // Return skill
    return skill;
}

int SkillSearch::getUserId() {
    // Return user ID
    return userId;
}

User SkillSearch::getUser() {
    // Get user
    User user = User(userId);

    // Return skill
    return user;
}

bool SkillSearch::getApprovedState() {
    // Return approved state
    return approvedState;
}

void SkillSearch::setApprovedState(bool newApprovedState) {
    // Update approved state
    approvedState = newApprovedState;
}

SkillSearch::SkillSearch(int _skillSearchId) {
    skillSearchId = _skillSearchId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill search select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Skills WHERE SkillId=?");

    // Execute query
    pstmt->setInt(1, skillId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get skill search details from row
    skillId = res->getInt("SkillId");
    userId = res->getInt("UserId");
    approvedState = res->getBoolean("ApprovedState");

    // Delete result from memory
    delete res;
}


SkillSearch::SkillSearch(Skill &skill, User &user) {
    // TODO: Create new skill search
}

SkillSearch::SkillSearch(int _skillSearchId, int _skillId, int _userId, bool _approvedState) {
    skillSearchId = _skillSearchId;
    skillId = _skillId;
    userId = _userId;
    approvedState = _approvedState;
}

SkillResult getSkillByName(std::string name) {
    SkillResult skillResult = SkillResult();

    // TODO: Get skill by name if it exists

    return skillResult;
}

std::vector<User> getApprovedApplicantsBySkill(std::string skillName) {
    std::vector<User> applicants;

    // TODO: Get approved applicants by approved searches

    return applicants;
}

std::vector<User> getApprovedCompaniesBySkill(std::string skillName) {
    std::vector<User> companies;

    // TODO: Get approved companies by approved searches

    return companies;
}

std::vector<SkillSearch> getUnapprovedApplicantSkillSearches() {
    std::vector<SkillSearch> skillSearches;

    // TODO: Get unapproved applicant skill searches

    return skillSearches;
}

std::vector<SkillSearch> getUnapprovedCompanySkillSearches() {
    std::vector<SkillSearch> skillSearches;

    // TODO: Get unapproved company skill searches

    return skillSearches;
}

SkillSearchResult getUserSkillSearch(User &user) {
    SkillSearchResult userSkillSearchResult = SkillSearchResult();

    // TODO: Get user's skill search

    return userSkillSearchResult;
}
