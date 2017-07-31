#include "interactive/IsRunningInteractively.h"

#include "3ds.h"
#include <cstdio>

namespace capture::interactive {
    bool IsRunningInteractively() {
        hidScanInput();
        return hidKeysDown() != 0;
    }
}
