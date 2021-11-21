// Include header file
#include "Utils/Result.hpp"

// Required headers
#include <string>

bool Result::getSuccess() {
    return success;
}

void Result::setSuccess(bool newSuccess) {
    success = newSuccess;
}

std::string Result::getErrorMsg() {
    return errorMsg;
}

void Result::setError(std::string newErrorMsg) {
    success = false;
    errorMsg = newErrorMsg;
}

Result::Result() {
    success = false;
}

Result::Result(std::string _errorMsg) {
    success = false;
    errorMsg = _errorMsg;
}
