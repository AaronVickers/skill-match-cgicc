// Include header file
#include "Utils/CGIComponent.hpp"

// Required headers
#include <ostream>

// Overload output stream operator
std::ostream& operator<<(std::ostream &os, const CGIComponent &obj) {
    // Render component
    obj.render(os);

    // Return stream
    return os;
}
