#include "RunAutomaticTests.h"

#include "RunTests.h"

namespace capture {
    int RunAutomaticTests(const config::CaptureConfig &config) {
        return RunTests(config.CreateCatchArgumentVector());
    }
}
