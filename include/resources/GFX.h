#ifndef CAPTURE_RESOURCES_GFX_H
#define CAPTURE_RESOURCES_GFX_H

#include "3ds/gfx.h"

namespace capture::resources {
    class GFX {

    public:
        GFX(GFX&&) = default;
        GFX& operator= (GFX&&) = default;
        GFX(const GFX&) = delete;
        GFX& operator= (const GFX&) = delete;


        GFX() {
            gfxInitDefault();
        }

        GFX (
                GSPGPU_FramebufferFormats topFormat,
                GSPGPU_FramebufferFormats bottomFormat,
                bool vrambuffers) 
        {
            gfxInit(topFormat, bottomFormat, vrambuffers);
        }

        void FlushBuffers() { gfxFlushBuffers(); }

        void ConfigScreen(gfxScreen_t scr, bool immediate) { gfxConfigScreen(scr, immediate); }

        uint8_t *GetFramebuffer(gfxScreen_t screen, gfx3dSide_t side, uint16_t *width, uint16_t *height) {
            return gfxGetFramebuffer(screen, side, width, height);
        }

        GSPGPU_FramebufferFormats GetScreenFormat(gfxScreen_t screen) { return gfxGetScreenFormat(screen); }

        bool Is3D() { return gfxIs3D(); }

        void Set3D(bool enable) { gfxSet3D(enable); }

        void SetDoubleBuffering(gfxScreen_t screen, bool doubleBuffering) {
            gfxSetDoubleBuffering(screen, doubleBuffering);
        }

        void SetScreenFormat(gfxScreen_t screen, GSPGPU_FramebufferFormats format) {
            gfxSetScreenFormat(screen, format);
        }

        void SwapBuffers() { gfxSwapBuffers(); }

        void SwapBuffersGpu() { gfxSwapBuffers(); }

        ~GFX() {
            gfxExit();
        }
    };
}

#endif // CAPTURE_RESOURCES_GFX_H
