// Header guard
#ifndef ROLES_H_
#define ROLES_H_

// Required headers
#include <string>

// Roles class structure
class Roles {
public:
    bool doesAdminExist();
};

// Role class structure
class Role {
private:
    int roleId;
    std::string name;
public:
    int getRoleId();
    std::string getName();

    Role(std::string name);
};

// End of header guard
#endif
