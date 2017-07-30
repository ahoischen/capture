#ifndef CAPTURE_CONFIG_CAPTUREKEYS_H
#define CAPTURE_CONFIG_CAPTUREKEYS_H

#include <memory>

#include "cpptoml.h"
#include "interactive/Keys.h"

namespace capture::config {
    class CaptureKeys {
        interactive::KeyState _startInteractive;
        interactive::KeyState _enableOption;
        interactive::KeyState _changeOption;
        interactive::KeyState _moveUp;
        interactive::KeyState _moveDown;
        interactive::KeyState _closeApplication;
        interactive::KeyState _startTests;

    public:
        interactive::KeyState GetStartInteractive() const { return _startInteractive; }
        interactive::KeyState GetEnableOption() const { return _enableOption; }
        interactive::KeyState GetChangeOption() const { return _changeOption; }
        interactive::KeyState GetMoveUp() const { return _moveUp; }
        interactive::KeyState GetMoveDown() const { return _moveDown; }
        interactive::KeyState GetCloseApplication() const { return _closeApplication; }
        interactive::KeyState GetStartTests() const { return _startTests; }

        void SetStartInteractive(interactive::KeyState value) { _startInteractive = value; }
        void SetEnableOption(interactive::KeyState value) { _enableOption = value; }
        void SetChangeOption(interactive::KeyState value) { _changeOption = value; }
        void SetMoveUp(interactive::KeyState value) { _moveUp = value; }
        void SetMoveDown(interactive::KeyState value) { _moveDown = value; }
        void SetCloseApplication(interactive::KeyState value) { _closeApplication = value; }
        void SetStartTests(interactive::KeyState value) { _startTests = value; }

        void InitFromTable(std::shared_ptr<cpptoml::table> table);
    };
}

#endif // CAPTURE_CONFIG_CAPTUREKEYS_H
