// Include header file
#include "Utils/Users.hpp"

int User::getUserId() {
    // Return username
    return userId;
}

std::string User::getUsername() {
    // Return username
    return username;
}

std::string User::getEmail() {
    // Return email
    return email;
}

int User::getRoleId() {
    // Return role
    return roleId;
}

Role User::getRole() {
    // TODO: Get user's role
    Role role = Role();

    // Return role
    return role;
}