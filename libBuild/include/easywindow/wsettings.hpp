#pragma once

#include <string>
#include <memory>

namespace ewin
{
    struct WindowSettings
    {
    public:
        std::shared_ptr<std::pair<int, int>> size;
        std::shared_ptr<std::pair<int, int>> pos;
        std::string title;
        float opacity;
    };
}