// Header guard
#ifndef SKILLS_H_
#define SKILLS_H_

// Required headers
#include <string>
#include <vector>
#include "Utils/Result.hpp"
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

    Skill(int _skillId);
    Skill(std::string _name);
    Skill(int _skillId, std::string _name);
};

// Skill result class structure
class SkillResult: public Result {
public:
    Skill *skill;
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

    void setApprovedState(bool newApprovedState);

    SkillSearch(int _skillSearchId);
    SkillSearch(Skill &skill, User &user);
    SkillSearch(int _skillSearchId, int _skillId, int _userId, bool _approvedState);
};

// Skill search result class structure
class SkillSearchResult: public Result {
public:
    SkillSearch *skillSearch;
};

// Skills namespace
namespace Skills {
    SkillResult getSkillByName(std::string name);

    std::vector<User> getApprovedApplicantsBySkill(std::string skillName);
    std::vector<User> getApprovedCompaniesBySkill(std::string skillName);

    std::vector<SkillSearch> getUnapprovedApplicantSkillSearches();
    std::vector<SkillSearch> getUnapprovedCompanySkillSearches();

    SkillSearchResult getUserSkillSearch(User &user);
}

// End of header guard
#endif
