// Header guard
#ifndef CGI_PAGE_H_
#define CGI_PAGE_H_

// Required headers
#include <ostream>
#include "Utils/CGIComponent.hpp"

// CGI page class structure
class CGIPage: public CGIComponent {
private:
    // Render method
    void render(std::ostream &os) const;
protected:
    // On GET request virtual method
    virtual void onGET(std::ostream &os) const;

    // On POST request virtual method
    virtual void onPOST(std::ostream &os) const;
};

// End of header guard
#endif
