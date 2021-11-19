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

    Skill(int skillId);
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
    Skill* getSkill();
    int getUserId();
    User* getUser();
    bool getApprovedState();

    bool setApprovedState(bool newApprovedState);

    SkillSearch(int skillSearchId);
    SkillSearch(Skill &skill, User &user);
};

// Skills class structure
class Skills {
public:
    Skill* getSkillBySkillId(int skillId);
    Skill* getSkillBySkillName(std::string name);

    SkillSearch getSkillSearchBySkillSearchId(int skillSearchId);

    std::vector<User*> getApprovedApplicantSkillSearchesBySkillName(std::string name);
    std::vector<User*> getApprovedCompanySkillSearchesBySkillName(std::string name);

    std::vector<SkillSearch*> getUnapprovedApplicantSkillSearches();
    std::vector<SkillSearch*> getUnapprovedCompanySkillSearches();

    SkillSearch* getUserSkillSearch(User &user);
};

// End of header guard
#endif
