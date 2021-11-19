// Header guard
#ifndef SKILLS_H_
#define SKILLS_H_

// Required headers
#include <string>
#include <vector>
#include "Utils/Users.hpp"

// Forward declaration of required classes
class User;

// Skill class structure
class Skill {
private:
    int skillId;
    std::string name;
public:
    int getSkillId();
    std::string getName();

    Skill(std::string name);
};

// SkillSearch class structure
class SkillSearch {
private:
    int skillSearchId;
    int skillId;
    int userId;
    bool approvedState;
public:
    int getSkillSearchId();
    int getSkillId();
    Skill getSkill();
    int getUserId();
    User getUser();
    bool getApprovedState();

    bool setApprovedState(bool newApprovedState);

    SkillSearch(Skill skill, User user);
};

// Skills class structure
class Skills {
public:
    std::vector<User> getApprovedApplicantSkillSearchesBySkill(Skill skill);

    std::vector<User> getApprovedCompanySkillSearchesBySkill(Skill skill);

    std::vector<SkillSearch> getUnapprovedApplicantSkillSearches();

    std::vector<SkillSearch> getUnapprovedCompanySkillSearches();

    SkillSearch getUserSkillSearch(User user);
};

// End of header guard
#endif
