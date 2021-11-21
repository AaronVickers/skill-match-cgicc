// Include header file
#include "Utils/Skills.hpp"

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

bool SkillSearch::setApprovedState(bool newApprovedState) {
    // Update approved state
    approvedState = newApprovedState;
}

SkillSearch::SkillSearch(int _skillSearchId) {

}

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
