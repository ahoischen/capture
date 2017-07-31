#ifndef CAPTURE_WRITEEXITCODE_H
#define CAPTURE_WRITEEXITCODE_H

#include <string>

namespace capture {
    // Writes the exit code of the program to a file on the sdmc for later
    // consumption by CI services.
    // A test run might later exit with this code itself to signal test success
    // or failure.
    void WriteExitCode(const int exit_code, const std::string filename);
}

#endif // CAPTURE_WRITEEXITCODE_H
