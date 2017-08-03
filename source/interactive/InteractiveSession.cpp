#include "capture/interactive/InteractiveSession.h"

#include <iostream>

#include "3ds.h"
#include "capture/interactive/ConfigMenu.h"
#include "capture/RunTests.h"

namespace capture::interactive {
    int InteractiveSession::Run(config::CaptureConfig& config, resources::GFX *gfx, resources::Console upperConsole) {
        resources::Console lowerConsole(GFX_BOTTOM);

        // Displays instructions on the bottom screen.
        ShowMenuButtons(config, lowerConsole);

        // All other output is to the top screen only.
        upperConsole.Select();
        
        bool applicationTerminated = ConfigMenu().Run(config, gfx);

        int catchExitCode{255};
        if (!applicationTerminated) {
            catchExitCode = RunTests(config.CreateCatchArgumentVector());
        }

        return catchExitCode;

        // Wait for the user to close the app.
        while(aptMainLoop()) {
            hidScanInput();
            if (hidKeysDown() & config.GetCaptureKeys().GetCloseApplication()) {
                break;
            }
            gfx->FlushBuffers();
            gfx->SwapBuffers();
            gspWaitForVBlank();
        }
    }

    void InteractiveSession::ShowMenuButtons(const config::CaptureConfig& config, resources::Console &lowerConsole) {
        lowerConsole.Select();
        auto keys = config.GetCaptureKeys();

        const char * instructionFormat = "[\e[32m%s\e[39m] %s\n";

        printf(instructionFormat, Keys::Name(keys.GetMoveUp()).c_str(), "Move Up");
        printf(instructionFormat, Keys::Name(keys.GetMoveDown()).c_str(), "Move Down");
        printf(instructionFormat, Keys::Name(keys.GetEnableOption()).c_str(), "Enable/Disable");
        printf(instructionFormat, Keys::Name(keys.GetChangeOption()).c_str(), "Change Value");
        printf(instructionFormat, Keys::Name(keys.GetCloseApplication()).c_str(), "Exit Application");
        printf(instructionFormat, Keys::Name(keys.GetStartTests()).c_str(), "Start Tests");
    }
}
