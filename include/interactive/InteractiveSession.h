#ifndef CATCH_INTERACTIVE_INTERACTIVESESSION_H
#define CATCH_INTERACTIVE_INTERACTIVESESSION_H

#include "config/CaptureConfig.h"
#include "interactive/Keys.h"
#include "resources/Console.h"
#include "resources/GFX.h"

namespace capture::interactive {
    class InteractiveSession {
        void ShowMenuButtons(const config::CaptureConfig &config, resources::Console &lowerConsole);

    public:
        int Run(config::CaptureConfig &config, resources::GFX *gfx, resources::Console upperConsole);
    };
}

#endif // CATCH_INTERACTIVE_INTERACTIVESESSION_H
