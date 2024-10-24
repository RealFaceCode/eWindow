#pragma once

#include <string>

namespace ewin
{
    struct WindowSettings
    {
        int width;
        int height;
        std::string title;
        float opacity;
    };
}