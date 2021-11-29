// Header guard
#ifndef LOG_OUT_BUTTON_CGI_COMPONENT_H_
#define LOG_OUT_BUTTON_CGI_COMPONENT_H_

// CGI component header
#include "Utils/CGIComponent.hpp"

// Log out button CGI component class structure
class LogOutButton: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
};

// End of header guard
#endif
