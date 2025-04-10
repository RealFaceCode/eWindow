#pragma once

#include "window.hpp"
#include "defines.hpp"
#include "contextsettings.hpp"

typedef void* (*loadproc)(const char *name);

namespace ewin
{
    EWIN_API bool Init();
    EWIN_API void Terminate();
    EWIN_API void HideTerminal();
    EWIN_API void ShowTerminal();
    EWIN_API void CloseTerminal();

    EWIN_API void SetOpenGLVersion(int major, int minor);           // 0 = default, set to 4.6
    EWIN_API void SetOpenGLProfile(OpenGLProfile profile);          // OpenGLProfileCore = default, set to OpenGLProfileCore
    EWIN_API void SetOpenGLForwardCompat(bool forward);             // false = default, set to true
    EWIN_API void SetOpenGLDebugContext(bool debug);                // false = default, set to false
    EWIN_API void SetOpenGLRobustness(bool robust);                 // false = default, set to false  // GLFW_NO_RESET_NOTIFICATION = 0, GLFW_LOSE_CONTEXT_ON_RESET = 0x00020007

    EWIN_API void SetBits(int red, int green, int blue, int alpha); // 0 = default, set to 8
    EWIN_API void SetDepthBits(int depth);                          // 0 = default, set to 24
    EWIN_API void SetStencilBits(int stencil);                      // 0 = default, set to 8
    EWIN_API void SetSamples(int samples);                          // 0 = default, set to 4
    EWIN_API void SetSRGB(bool srgb);                               // false = default, set to true
    EWIN_API void SetDoubleBuffer(bool doubleBuffer);               // true = default, set to true
    EWIN_API void SetVSync(bool vsync);                             // true = default

    EWIN_API ContextSettings& GetContextSettings();

    EWIN_API loadproc GetProcAddress(const char* name);
}