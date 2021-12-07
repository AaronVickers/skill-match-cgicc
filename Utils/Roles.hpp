// Header guard
#ifndef ROLES_H_
#define ROLES_H_

// Define environment constants
#define ADMINISTRATOR_ROLE_NAME "Administrator"
#define APPLICANT_ROLE_NAME "Applicant"
#define COMPANY_ROLE_NAME "Company"

// Required headers
#include <string>
#include "Utils/Result.hpp"

// Role class structure
class Role {
private:
    int roleId;
    std::string name;
public:
    int getRoleId();
    std::string getName();

    Role(int _roleId);
    Role(int _roleId, std::string _name);
};

// Role result class structure
class RoleResult: public Result {
public:
    Role *role;
};

// Roles namespace
namespace Roles {
    RoleResult getRoleByName(std::string name);
}

// End of header guard
#endif
