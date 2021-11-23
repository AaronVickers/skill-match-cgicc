// Include header file
#include "Utils/Skills.hpp"

// Initialised MySQL header
#include "Utils/MySqlInit.hpp"

// MySQL headers
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

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

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

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
    name = res->getString("Name");

    // Delete result from memory
    delete res;
}

Skill::Skill(std::string _name) {
    
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

    // Initialise MySQL connection
    MySqlInit db = MySqlInit();

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

/*
SkillSearch::SkillSearch(Skill &skill, User &user) {

}

SkillSearch Skills::getSkillByName(std::string name) {

}

std::vector<User> Skills::getApprovedApplicantSkillSearchesByName(std::string name) {
    
}

std::vector<User> Skills::getApprovedCompanySkillSearchesByName(std::string name) {
    
}

std::vector<SkillSearch> Skills::getUnapprovedApplicantSkillSearches() {
    
}

std::vector<SkillSearch> Skills::getUnapprovedCompanySkillSearches() {
    
}

SkillSearch Skills::getUserSkillSearch(User &user) {
    
}
*/
