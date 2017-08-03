#include "capture/RunAutomaticTests.h"

#include "capture/RunTests.h"

namespace capture {
    int RunAutomaticTests(const config::CaptureConfig &config) {
        return RunTests(config.CreateCatchArgumentVector());
    }
}
