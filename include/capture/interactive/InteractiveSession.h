#ifndef CATCH_INTERACTIVE_INTERACTIVESESSION_H
#define CATCH_INTERACTIVE_INTERACTIVESESSION_H

#include "capture/config/CaptureConfig.h"
#include "capture/interactive/Keys.h"
#include "capture/resources/Console.h"
#include "capture/resources/GFX.h"

namespace capture::interactive {
    class InteractiveSession {
        void ShowMenuButtons(const config::CaptureConfig &config, resources::Console &lowerConsole);

    public:
        int Run(config::CaptureConfig &config, resources::GFX *gfx, resources::Console upperConsole);
    };
}

#endif // CATCH_INTERACTIVE_INTERACTIVESESSION_H
