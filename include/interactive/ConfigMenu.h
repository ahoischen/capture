#ifndef CAPTURE_INTERACTIVE_CONFIGMENU_H
#define CAPTURE_INTERACTIVE_CONFIGMENU_H

#include "config/CaptureConfig.h"
#include "interactive/Keys.h"
#include "resources/GFX.h"

namespace capture::interactive {
    class ConfigMenu {
        bool MoveCursor(KeyState kDown, const config::CaptureConfig &config, size_t &cursor);
        bool ModifyOption(KeyState kDown, config::CaptureConfig &config, size_t &cursor);
        bool UserWantsToStart(KeyState kDown, const config::CaptureConfig &config);
        bool UserWantsToTerminate(KeyState kDown, const config::CaptureConfig &config);
        void RedrawMenu(const config::CaptureConfig &config, size_t cursor);
    
    public:
        bool Run(config::CaptureConfig &config, resources::GFX *gfx);
    };
}

#endif // CAPTURE_INTERACTIVE_CONFIGMENU_H
