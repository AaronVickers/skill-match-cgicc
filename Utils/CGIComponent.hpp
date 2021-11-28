// Header guard
#ifndef CGI_COMPONENT_H_
#define CGI_COMPONENT_H_

// CGICC headers
#include "cgicc/Cgicc.h"
#include "cgicc/CgiEnvironment.h"

// Required headers
#include <ostream>

// CGI component class structure
class CGIComponent {
private:
    // Render pure virtual method
    virtual void render(std::ostream &os) const = 0;
protected:
    // Cgicc instance
    cgicc::Cgicc cgi;

    // Cgicc environment
    const cgicc::CgiEnvironment &env = CGIComponent::cgi.getEnvironment();
public:
    // Overload output stream operator
    friend std::ostream& operator<<(std::ostream &os, const CGIComponent &obj);
};

// End of header guard
#endif
