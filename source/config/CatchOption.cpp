#include "capture/config/CatchOption.h"

#include <cstdio>

#include "capture/config/CaptureConfig.h"

namespace capture::config {
    void CatchOption::DrawMenuString(const CaptureConfig &config) {
        printf("%s%s%s\n",
            GetIsEnabled() ? "\e[0m" : "\e[0;2m",
            GetName().c_str(),
            "\e[39m");
    }

    bool CatchOption::ProcessInputs(interactive::KeyState kDown, CaptureConfig &config) {
        bool modified{false};
        
        if (kDown & config.GetCaptureKeys().GetEnableOption()) {
            // Toggle IsEnabled.
            SetIsEnabled(!GetIsEnabled());
            modified = true;
        }

        return modified;
    }
}
