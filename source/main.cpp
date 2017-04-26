#include "capture.hpp"

void SetupTestingEnvironment() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    gspWaitForVBlank();
}

void TearDownTestingEnvironment() {
    gfxExit();
}
