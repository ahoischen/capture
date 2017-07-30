#ifndef CAPTURE_RESOURCES_CONSOLE_H
#define CAPTURE_RESOURCES_CONSOLE_H

#include "3ds/console.h"

namespace capture::resources {
    class Console {
        PrintConsole _console;

    public:
        Console(Console&&) = default;
        Console& operator= (Console&&) = default;
        Console(const Console&) = delete;
        Console& operator= (const Console&) = delete;

        const PrintConsole *GetConsole() const { return &_console; }
        PrintConsole *GetConsole() { return &_console; } 

        Console() : _console(*consoleGetDefault()) {}

        Console(gfxScreen_t screen) { consoleInit(screen, &_console); }

        void Select() { consoleSelect(&_console); }

        void SetFont(ConsoleFont* font) { consoleSetFont(&_console, font); }

        void SetWindow(int x, int y, int width, int height) {
            consoleSetWindow(&_console, x, y, width, height);
        }

        ~Console() = default;

        static void DebugInit(debugDevice device) { consoleDebugInit(device); }
    };
}

#endif // CAPTURE_RESOURCES_CONSOLE_H
