// Include header file
#include "Components/RegisterForm.hpp"

// CGICC headers
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>
#include "Utils/Users.hpp"

// Render method
void RegisterForm::render(std::ostream &os) const {
    // Form for registration
    auto registerForm = cgicc::form();
    registerForm.set("method", "post"); // Submit as post request

    // Username input
    registerForm.add(cgicc::div()
        .add(cgicc::span("Username: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "username")
            .set("placeholder", "Username")
        )
    );

    // Email input
    registerForm.add(cgicc::div()
        .add(cgicc::span("Email: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "email")
            .set("placeholder", "Email")
        )
    );

    // Password input
    registerForm.add(cgicc::div()
        .add(cgicc::span("Password: "))
        .add(cgicc::input()
            .set("type", "password")
            .set("name", "password")
            .set("placeholder", "Password")
        )
    );

    // Skill input
    registerForm.add(cgicc::div()
        .add(cgicc::span("Skill: "))
        .add(cgicc::input()
            .set("type", "text")
            .set("name", "skill")
            .set("placeholder", "Skill")
        )
    );

    // Create roles selection
    auto roleSelect = cgicc::select();
    roleSelect.set("name", "role");

    // Add admin to roles selection if admin doesn't exist
    if (!Users::doesAdminExist()) {
        roleSelect.add(cgicc::option("Administrator")
            .set("value", "Administrator")
        );
    }

    // Add applicant to roles selection
    roleSelect.add(cgicc::option("Applicant")
        .set("value", "Applicant")
    );

    // Add company to roles selection
    roleSelect.add(cgicc::option("Company")
        .set("value", "Company")
    );

    // Role dropdown
    registerForm.add(cgicc::div()
        .add(cgicc::span("Role: "))
        .add(roleSelect)
    );

    // Register button
    registerForm.add(cgicc::input()
        .set("type", "submit")
        .set("value", "Register")
    );

    // Login link
    registerForm.add(cgicc::a("Login")
        .set("href", "./login.cgi")
    );

    // Render register form
    os << registerForm;
}