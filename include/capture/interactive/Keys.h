#ifndef CAPTURE_INTERACTIVE_BUTTONS_H
#define CAPTURE_INTERACTIVE_BUTTONS_H

#include <map>

#include "3ds.h"

namespace capture::interactive {
    typedef uint32_t KeyState;

    class Keys {
    public:
        // Gets the string representation of the given key pattern.
        static std::string Name(KeyState state);

        // Gets the bitfield for the named key.
        static KeyState State(const std::string &name);
    };
}

#endif // CAPTURE_INTERACTIVE_BUTTONS_H
