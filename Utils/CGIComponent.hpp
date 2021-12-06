// Header guard
#ifndef CGI_COMPONENT_H_
#define CGI_COMPONENT_H_

// Required headers
#include <ostream>

// CGI component class structure
class CGIComponent {
private:
    // Render pure virtual method
    virtual void render(std::ostream &os) const = 0;
public:
    // Overload output stream operator
    friend std::ostream& operator<<(std::ostream &os, const CGIComponent &obj);
};

// End of header guard
#endif
