#include "capture/interactive/SoftwareKeyboard.h"

#include <cassert>
#include <cstring>

namespace capture::interactive {
    void SoftwareKeyboard::SetInitialText(const std::string &text) {
        // The initial text is stored in this object to avoid use-after-free.
        strncpy(_initialText.get(), text.c_str(), _maxTextLength);
        swkbdSetInitialText(&_state, _initialText.get());
    }

    void SoftwareKeyboard::SetButton(SwkbdButton button, const std::string &text, bool submit) {
        // SWKBD_BUTTON_NONE is 4 and other values are not defined.
        size_t buttonIndex = static_cast<size_t>(button);
        assert(buttonIndex < 3);

        // The button labels are stored in this object to avoid use-after-free.
        _buttonText[button] = std::make_unique<char[]>(text.length()+1);        
        strncpy(_buttonText[button].get(), text.c_str(), text.length());

        swkbdSetButton(&_state, button, _buttonText[button].get(), submit);
    }

    SwkbdButton SoftwareKeyboard::InputText(std::string &output) {
        // A temprary buffer for the C string is allocated.
        std::unique_ptr<char[]> buffer(std::make_unique<char[]>(_maxTextLength+1));

        // Length restriction includes the null terminator, so the full buffer
        // can be used.
        auto button = swkbdInputText(&_state, buffer.get(), _maxTextLength+1);

        // The output string is reconstructed from the buffer.
        output = std::string{buffer.get()};

        return button;
    }
}
