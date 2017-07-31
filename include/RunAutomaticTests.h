#ifndef CAPTURE_RUNAUTOMATICTESTS_H
#define CAPTURE_RUNAUTOMATICTESTS_H

#include "config/CaptureConfig.h"

namespace capture {
    // Executes the default tests. Assumes to be on citra, if unable to detect the
    // platform. Writes test output to a file on the sdmc for later use and runs
    // tests tagged with the platform's default string.
    int RunAutomaticTests(const config::CaptureConfig &config);
}

#endif // CAPTURE_RUNAUTOMATICTESTS_H
