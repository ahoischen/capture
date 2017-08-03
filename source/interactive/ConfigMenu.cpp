#include "capture/interactive/ConfigMenu.h"

#include <algorithm>
#include <cstdio>
#include <iostream>

namespace capture::interactive {

    bool ConfigMenu::Run(config::CaptureConfig &config, resources::GFX *gfx) {
        size_t cursor{0};

        RedrawMenu(config, cursor);

        while (aptMainLoop()) {
            hidScanInput();
            KeyState kDown = hidKeysDown();
            bool menuChanged{false};

            menuChanged |= MoveCursor(kDown, config, cursor);

            menuChanged |= ModifyOption(kDown, config, cursor);

            if (menuChanged) {
                RedrawMenu(config, cursor);
            }

            if (UserWantsToStart(kDown, config)) {
                return false;
            }
            
            if (UserWantsToTerminate(kDown, config)) {
                return true;
            }

            gfx->SwapBuffers();
            gfx->FlushBuffers();
            gspWaitForVBlank();
        }

        return true;
    }

    // Processes potential movement of the cursor depending on the button presses.
    // Returns if it has moved the cursor or not.
    bool ConfigMenu::MoveCursor(KeyState kDown, const config::CaptureConfig &config, size_t &cursor) {
        bool moved{false};
        
        if (kDown & config.GetCaptureKeys().GetMoveDown()) {
            ++cursor;
            cursor %= config.GetOptions().size();
            moved = true;
        }

        if (kDown & config.GetCaptureKeys().GetMoveUp()) {
            // Intentionally allows underflows
            --cursor;

            // Picks either the cursor's new, decremented value or the last position
            // in the list if the cursor has underflowed. 
            cursor = std::min(cursor, config.GetOptions().size()-1);

            moved = true;
        }

        return moved;
    }

    bool ConfigMenu::ModifyOption(KeyState kDown, config::CaptureConfig &config, size_t &cursor){
        return config.GetOptions()[cursor].get()->ProcessInputs(kDown, config);
    }

    bool ConfigMenu::UserWantsToStart(KeyState kDown, const config::CaptureConfig &config) {
        return kDown & config.GetCaptureKeys().GetStartTests();
    }

    bool ConfigMenu::UserWantsToTerminate(KeyState kDown, const config::CaptureConfig &config) {
        return kDown & config.GetCaptureKeys().GetStartTests();
    }

    void ConfigMenu::RedrawMenu(const config::CaptureConfig &config, size_t cursor) {
        // Clear the screen
        std::cout << "\e[2J\e[;H";

        // Display each Option
        for (size_t i = 0; i < config.GetOptions().size(); ++i) {
            // Print the cursor
            std::cout << ((i == cursor) ? "\e[0;32m>\e[39m" : " ");

            config.GetOptions()[i].get()->DrawMenuString(config);
        }
        
        // Draw the cursor on the 6th line from the start
    }
}
