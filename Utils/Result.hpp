// Header guard
#ifndef RESULT_H_
#define RESULT_H_

// Required headers
#include <string>

// Result class structure
class Result {
private:
    // Success status
    bool success;

    // Error message
    std::string errorMsg;
public:
    bool getSuccess();
    std::string getErrorMsg();

    void setSuccess(bool newSuccess);
    void setError(std::string newErrorMsg);

    Result();
    Result(std::string _errorMsg);
};

// End of header guard
#endif
