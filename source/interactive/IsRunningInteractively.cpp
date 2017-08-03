#include "capture/interactive/IsRunningInteractively.h"

#include "3ds.h"
#include <cstdio>
#include <iostream>
#include <string>

namespace capture::interactive {
    bool IsRunningInteractively() {
        bool ButtonPressed{false};
        std::cout << "Press any button to launch interactive mode." << std::endl;

        // Citra is weird abouts it's inputs. This should clear the initial random value.
        hidScanInput();

        // Citra only properly detects inputs in an aptMainLoop. The user has 120 frames(2 seconds)
        // to initiate an interactive session.
        for(int i = 0; i < 120 && aptMainLoop() && !ButtonPressed; ++i) {            
            hidScanInput();
            ButtonPressed = hidKeysDown() != 0;
            gspWaitForVBlank();
        }

        return ButtonPressed;
    }
}
