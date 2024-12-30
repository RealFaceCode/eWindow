#pragma once
#include "defines.hpp"

namespace ewin
{
    struct ContextSettings
    {
        int major               = 4;
        int minor               = 6;
        OpenGLProfile profile   = OpenGLProfile::OpenGLProfileCore;
        bool forwardCompat      = true;
        bool debugContext       = false;
        bool robustness         = false;
        int redBits             = 8;
        int greenBits           = 8;
        int blueBits            = 8;
        int alphaBits           = 8;
        int depthBits           = 24;
        int stencilBits         = 8;
        int samples             = 4;
        bool srgb               = true;
        bool doubleBuffer       = true;
        bool vsync              = true;
    };
}