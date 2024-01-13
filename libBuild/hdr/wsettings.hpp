#pragma once

#include <string>

namespace ewin::structs
{
    struct WindowSettings
    {
        int width;
        int height;
        std::string title;
        float opacity;
    };
}