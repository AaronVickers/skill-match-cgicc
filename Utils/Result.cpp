// Include header file
#include "Utils/Result.hpp"

// Required headers
#include <string>

bool Result::getSuccess() {
    // Return success
    return success;
}

void Result::setSuccess(bool newSuccess) {
    // Update success
    success = newSuccess;
}

std::string Result::getErrorMsg() {
    // Return error message
    return errorMsg;
}

void Result::setError(std::string newErrorMsg) {
    // Set success to false and update error message
    success = false;
    errorMsg = newErrorMsg;
}

Result::Result() {
    // Initialise success as false
    success = false;
}

Result::Result(std::string _errorMsg) {
    // Initialise success as false with error message
    success = false;
    errorMsg = _errorMsg;
}
