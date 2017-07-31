#include "config/CatchValueOption.h"

#include <cstdio>
#include <memory>

#include "3ds.h"
#include "config/CaptureConfig.h"
#include "interactive/SoftwareKeyboard.h"

namespace capture::config {
    void CatchValueOption::DrawMenuString(const CaptureConfig &config) {
        printf("%s%s: %s%s%s\n",
            GetIsEnabled() ? "\e[0m" : "\e[0;2m",
            GetName().c_str(),
            "\e[33m",
            GetValue().c_str(),
            "\e[39m");
    }

    bool CatchValueOption::ProcessInputs(interactive::KeyState kDown, CaptureConfig &config) {
        bool modified{CatchOption::ProcessInputs(kDown, config)};

        if (kDown & config.GetCaptureKeys().GetChangeOption()) {
            std::unique_ptr<interactive::SoftwareKeyboard> keyboard;
            keyboard = std::make_unique<interactive::SoftwareKeyboard>(SWKBD_TYPE_NORMAL, 1, 0x400);
            
            keyboard->SetInitialText(GetValue());
            keyboard->SetButton(SWKBD_BUTTON_CONFIRM, "Confirm", true);

            std::string newValue{""};
            auto pressedButton = keyboard->InputText(newValue);

            if (pressedButton != SWKBD_BUTTON_NONE) {
                SetValue(newValue);

                modified = true;
            }
        }

        return modified;
    }
}
