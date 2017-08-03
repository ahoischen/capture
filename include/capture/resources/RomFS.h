#ifndef CAPTURE_RESOURCES_ROMFS_H
#define CAPTURE_RESOURCES_ROMFS_H

#include "3ds/romfs.h"

namespace capture::resources {

    // Manages access to RomFS.
    class RomFS {

    public:
        RomFS(RomFS&&) = default;
        RomFS& operator= (RomFS&&) = default;
        RomFS(const RomFS&) = delete;
        RomFS& operator= (const RomFS&) = delete;

        RomFS() {
            romfsInit();
        }
        
        ~RomFS() {
            romfsExit();
        }
    };
}

#endif // CAPTURE_RESOURCES_ROMFS_H
