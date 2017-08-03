#ifndef CAPTURE_INTERACTIVE_SOFTWAREKEYBOARD_H
#define CAPTURE_INTERACTIVE_SOFTWAREKEYBOARD_H

#include <memory>
#include <string>

#include "3ds.h"


namespace capture::interactive {

    // Represents a software keyboard.
    // This is a wrapper around 3ds/applets/swkbd.h, which hides all C strings.
    class SoftwareKeyboard {
        SwkbdState _state;
        SwkbdStatusData _statusData;
        SwkbdLearningData _learningData;
        size_t _maxTextLength;
        std::unique_ptr<char[]> _initialText;
        std::unique_ptr<char[]> _buttonText[3];

    public:
        // A software keyboard cannot be copied, because it has members that cannot
        // be copied either.
        SoftwareKeyboard(SoftwareKeyboard&&) = default;
        SoftwareKeyboard& operator= (SoftwareKeyboard&&) = default;
        SoftwareKeyboard(const SoftwareKeyboard&) = delete;
        SoftwareKeyboard& operator= (const SoftwareKeyboard&) = delete;

        SoftwareKeyboard(SwkbdType type, int numButtons, size_t maxTextLength):
            _maxTextLength(maxTextLength), 
            _initialText(std::make_unique<char[]>(_maxTextLength+1))
        {
            swkbdInit(&_state, type, numButtons, static_cast<int>(maxTextLength));
        }

        void SetInitialText(const std::string &text);

        void SetButton(SwkbdButton button, const std::string &text, bool submit);

        SwkbdButton InputText(std::string &output);
    };
}

#endif // CAPTURE_INTERACTIVE_SOFTWAREKEYBOARD_H
