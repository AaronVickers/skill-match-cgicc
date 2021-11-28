// Include header file
#include "Utils/CGIPage.hpp"

// CGICC headers
#include "cgicc/HTTPStatusHeader.h"
#include "cgicc/HTMLClasses.h"

// Required headers
#include <ostream>

// Not found response
void notFound(std::ostream &os) {
    // Error response data
    os << cgicc::HTTPStatusHeader(404, "Not Found") << std::endl;
    os << cgicc::html() << cgicc::head(cgicc::title("404 Not Found")) << std::endl;
    os << cgicc::body();

    // Error heading
    os << cgicc::h1("Not Found");

    // End of response
    os << cgicc::body() << cgicc::html();
}

// Render method
void CGIPage::render(std::ostream &os) const {
    // Handle different request types
    if (env.getRequestMethod() == "GET") {
        // Handle GET request
        onGET(os);
    } else if (env.getRequestMethod() == "POST") {
        // Handle POST request
        onPOST(os);
    }
}

// On GET request virtual method
void CGIPage::onGET(std::ostream &os) const {
    // Default to not found
    notFound(os);
}

// On POST request virtual method
void CGIPage::onPOST(std::ostream &os) const {
    // Default to not found
    notFound(os);
}
