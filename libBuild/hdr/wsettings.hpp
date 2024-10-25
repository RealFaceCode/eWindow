#pragma once

#include <string>

namespace ewin
{
    struct WindowSettings
    {
        int width;
        int height;
        int fbWidth;
        int fbHeight;
        int xpos;
        int ypos;
        std::string title;
        float opacity;
    };
}