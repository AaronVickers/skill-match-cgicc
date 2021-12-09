// Required headers
#include <iostream>
#include "Utils/Authentication.hpp"

// Use required namespaces
using namespace std;

// Entry function
int main(int argc, char *argv[]) {
    // Get current time
    time_t timeNow = time(0);
    // Round time down to multiple of 30
    time_t timeSeed = timeNow - timeNow % 30;

    // Generate TOTP code for current time
    string totpCode = Authentication::generateTOTPCode(timeSeed, 6);

    cout << "TOTP Code: " << totpCode << endl;

    return 0;
}
