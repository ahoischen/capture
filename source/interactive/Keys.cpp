#include "capture/interactive/Keys.h"

namespace capture::interactive {

    // Stores two maps for translating between strings and bitfields.
    // This approach is pretty messy due to the fact that I got unreadable linker errors
    // when just using a static class with static mambers.
    struct KeyStorage {
        bool _init;
        std::map<KeyState, std::string> _keyNames;
        std::map<std::string, KeyState> _keyStates;

        void Insert(KeyState state, std::string name) {
            _keyNames.insert(std::make_pair(state, name));
            _keyStates.insert(std::make_pair(name, state));
        }

        // Initializes the internal maps.
        void Initialize();
    } _keys;

    void KeyStorage::Initialize() {
        // CPad directions aren't implemented because they'd complicate naming and are generally confusing.
        Insert(KEY_A, "A");
        Insert(KEY_B, "B");
        Insert(KEY_SELECT, "Select");
        Insert(KEY_START, "Start");
        Insert(KEY_RIGHT, "Right");
        Insert(KEY_LEFT, "Left");
        Insert(KEY_UP, "Up");
        Insert(KEY_DOWN, "Down");
        Insert(KEY_R, "R");
        Insert(KEY_L, "L");
        Insert(KEY_X, "X");
        Insert(KEY_Y, "Y");
        Insert(KEY_ZL, "ZL");
        Insert(KEY_ZR, "ZR");
        Insert(KEY_TOUCH, "Touch");
        Insert(KEY_CSTICK_RIGHT, "CRight");
        Insert(KEY_CSTICK_LEFT, "CLeft");
        Insert(KEY_CSTICK_UP, "CUp");
        Insert(KEY_CSTICK_DOWN, "CDown");

        Insert(0xFFFFFFFF, "Any");

        _init = true;
    }
    
    KeyState Keys::State (const std::string &name) {
        if (!_keys._init) {
            _keys.Initialize();
        }
        
        return _keys._keyStates[name];
    }

    std::string Keys::Name(KeyState state){
        if (!_keys._init) {
            _keys.Initialize();
        }

        return _keys._keyNames[state];
    }
}
